#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Basic kqueue-based event loop library */

/* Data structure definitions */
#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0       /* No events registered */
#define AE_READABLE 1   /* Fire when descriptor is readable */
#define AE_WRITABLE 2   /* Fire when descriptor is writable */

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT 4

#define AE_NOMORE -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

/* Forward declarations */
typedef struct aeEventLoop aeEventLoop;

/* Types and data structures */
/* File event structure */
typedef struct aeFileEvent {
    int mask; /* one of AE_(READABLE|WRITABLE) */
    void *clientData;
    void (*rfileProc)(aeEventLoop *eventLoop, int fd, void *clientData, int mask);
    void (*wfileProc)(aeEventLoop *eventLoop, int fd, void *clientData, int mask);
} aeFileEvent;

/* A fired event */
typedef struct aeFiredEvent {
    int fd;
    int mask;
} aeFiredEvent;

/* Time event structure */
typedef struct aeTimeEvent {
    long long id; /* time event identifier. */
    long when_sec; /* seconds */
    long when_ms; /* milliseconds */
    void *clientData;
    struct aeTimeEvent *next;
    int (*timeProc)(aeEventLoop *eventLoop, long long id, void *clientData);
} aeTimeEvent;

/* State of an event based program */
struct aeEventLoop {
    int maxfd;   /* highest file descriptor currently registered */
    int setsize; /* max number of file descriptors tracked */
    long long timeEventNextId;
    aeFileEvent *events; /* Registered events */
    aeFiredEvent *fired; /* Fired events */
    aeTimeEvent *timeEventHead;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    void (*beforeSleep)(aeEventLoop *eventLoop);
};

/* API specific data */
typedef struct aeApiState {
    int kqfd;
    struct kevent *events;
} aeApiState;

/* Prototypes */
aeEventLoop *aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop *eventLoop);
void aeStop(aeEventLoop *eventLoop);
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        void (*proc)(aeEventLoop *eventLoop, int fd, void *clientData, int mask), void *clientData);
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        int (*proc)(aeEventLoop *eventLoop, long long id, void *clientData),
        void *clientData);
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);
int aeProcessEvents(aeEventLoop *eventLoop, int flags);
void aeMain(aeEventLoop *eventLoop);
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, void (*beforesleep)(aeEventLoop *eventLoop));
int aeGetSetSize(aeEventLoop *eventLoop);
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize);

/* kqueue specific implementation */

static int aeApiCreate(aeEventLoop *eventLoop) {
    aeApiState *state = malloc(sizeof(aeApiState));
    if (!state) return -1;

    /* Create the kqueue instance */
    state->kqfd = kqueue();
    if (state->kqfd == -1) {
        free(state);
        return -1;
    }

    /* Allocate memory for events */
    state->events = malloc(sizeof(struct kevent) * eventLoop->setsize);
    if (!state->events) {
        close(state->kqfd);
        free(state);
        return -1;
    }

    eventLoop->apidata = state;
    return 0;
}

static int aeApiResize(aeEventLoop *eventLoop, int setsize) {
    aeApiState *state = eventLoop->apidata;
    
    state->events = realloc(state->events, sizeof(struct kevent) * setsize);
    if (!state->events) return -1;
    
    return 0;
}

static void aeApiFree(aeEventLoop *eventLoop) {
    aeApiState *state = eventLoop->apidata;
    
    close(state->kqfd);
    free(state->events);
    free(state);
}

static int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask) {
    aeApiState *state = eventLoop->apidata;
    struct kevent ke;
    
    /* Register for read event */
    if (mask & AE_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }
    
    /* Register for write event */
    if (mask & AE_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(state->kqfd, &ke, 1, NULL, 0, NULL) == -1) return -1;
    }
    
    return 0;
}

static void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int mask) {
    aeApiState *state = eventLoop->apidata;
    struct kevent ke;
    
    /* Unregister read event */
    if (mask & AE_READABLE) {
        EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }
    
    /* Unregister write event */
    if (mask & AE_WRITABLE) {
        EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        kevent(state->kqfd, &ke, 1, NULL, 0, NULL);
    }
}

static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) {
    aeApiState *state = eventLoop->apidata;
    int retval, numevents = 0;
    
    /* If tvp is NULL, the kevent call will block indefinitely */
    if (tvp != NULL) {
        struct timespec timeout;
        timeout.tv_sec = tvp->tv_sec;
        timeout.tv_nsec = tvp->tv_usec * 1000;
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize, &timeout);
    } else {
        retval = kevent(state->kqfd, NULL, 0, state->events, eventLoop->setsize, NULL);
    }
    
    if (retval > 0) {
        int j;
        
        /* Process all events */
        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct kevent *e = state->events + j;
            
            if (e->filter == EVFILT_READ) mask |= AE_READABLE;
            if (e->filter == EVFILT_WRITE) mask |= AE_WRITABLE;
            eventLoop->fired[j].fd = e->ident;
            eventLoop->fired[j].mask = mask;
        }
    }
    
    return numevents;
}

/* Create event loop */
aeEventLoop *aeCreateEventLoop(int setsize) {
    aeEventLoop *eventLoop;
    int i;
    
    if ((eventLoop = malloc(sizeof(*eventLoop))) == NULL) goto err;
    
    eventLoop->events = malloc(sizeof(aeFileEvent) * setsize);
    eventLoop->fired = malloc(sizeof(aeFiredEvent) * setsize);
    
    if (eventLoop->events == NULL || eventLoop->fired == NULL) goto err;
    
    eventLoop->setsize = setsize;
    eventLoop->timeEventHead = NULL;
    eventLoop->timeEventNextId = 0;
    eventLoop->stop = 0;
    eventLoop->maxfd = -1;
    eventLoop->beforeSleep = NULL;
    
    if (aeApiCreate(eventLoop) == -1) goto err;
    
    /* Initialize file event structures */
    for (i = 0; i < setsize; i++) {
        eventLoop->events[i].mask = AE_NONE;
    }
    
    return eventLoop;

err:
    if (eventLoop) {
        free(eventLoop->events);
        free(eventLoop->fired);
        free(eventLoop);
    }
    return NULL;
}

/* Delete event loop */
void aeDeleteEventLoop(aeEventLoop *eventLoop) {
    aeApiFree(eventLoop);
    free(eventLoop->events);
    free(eventLoop->fired);
    free(eventLoop);
}

/* Stop event loop */
void aeStop(aeEventLoop *eventLoop) {
    eventLoop->stop = 1;
}

/* Register file event */
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask,
        void (*proc)(aeEventLoop *eventLoop, int fd, void *clientData, int mask), void *clientData) {
    if (fd >= eventLoop->setsize) {
        errno = ERANGE;
        return AE_ERR;
    }
    
    aeFileEvent *fe = &eventLoop->events[fd];
    
    if (aeApiAddEvent(eventLoop, fd, mask) == -1)
        return AE_ERR;
    
    fe->mask |= mask;
    if (mask & AE_READABLE) fe->rfileProc = proc;
    if (mask & AE_WRITABLE) fe->wfileProc = proc;
    fe->clientData = clientData;
    
    if (fd > eventLoop->maxfd)
        eventLoop->maxfd = fd;
    
    return AE_OK;
}

/* Delete file event */
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask) {
    if (fd >= eventLoop->setsize) return;
    
    aeFileEvent *fe = &eventLoop->events[fd];
    
    if (fe->mask == AE_NONE) return;
    
    aeApiDelEvent(eventLoop, fd, mask);
    
    fe->mask = fe->mask & (~mask);
    if (fd == eventLoop->maxfd && fe->mask == AE_NONE) {
        /* Update maxfd */
        int j;
        
        for (j = eventLoop->maxfd-1; j >= 0; j--)
            if (eventLoop->events[j].mask != AE_NONE) break;
        eventLoop->maxfd = j;
    }
}

/* Get file events */
int aeGetFileEvents(aeEventLoop *eventLoop, int fd) {
    if (fd >= eventLoop->setsize) return 0;
    aeFileEvent *fe = &eventLoop->events[fd];
    
    return fe->mask;
}

/* Get current time in milliseconds */
static long long aeGetTime(void) {
    struct timeval tv;
    long long retval;
    
    gettimeofday(&tv, NULL);
    retval = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return retval;
}

/* Create time event */
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds,
        int (*proc)(aeEventLoop *eventLoop, long long id, void *clientData),
        void *clientData) {
    long long id = eventLoop->timeEventNextId++;
    aeTimeEvent *te;
    
    te = malloc(sizeof(*te));
    if (te == NULL) return AE_ERR;
    
    te->id = id;
    long long now = aeGetTime();
    te->when_sec = now / 1000 + milliseconds / 1000;
    te->when_ms = now % 1000 + milliseconds % 1000;
    if (te->when_ms >= 1000) {
        te->when_sec++;
        te->when_ms -= 1000;
    }
    te->timeProc = proc;
    te->clientData = clientData;
    te->next = eventLoop->timeEventHead;
    eventLoop->timeEventHead = te;
    
    return id;
}

/* Delete time event */
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id) {
    aeTimeEvent *te = eventLoop->timeEventHead;
    aeTimeEvent *prev = NULL;
    
    while(te) {
        if (te->id == id) {
            if (prev == NULL)
                eventLoop->timeEventHead = te->next;
            else
                prev->next = te->next;
                
            free(te);
            return AE_OK;
        }
        prev = te;
        te = te->next;
    }
    
    return AE_ERR; /* NO event with the specified ID found */
}

/* Search the nearest timer */
static aeTimeEvent *aeSearchNearestTimer(aeEventLoop *eventLoop) {
    aeTimeEvent *te = eventLoop->timeEventHead;
    aeTimeEvent *nearest = NULL;
    
    while(te) {
        if (!nearest || te->when_sec < nearest->when_sec ||
                (te->when_sec == nearest->when_sec &&
                 te->when_ms < nearest->when_ms))
            nearest = te;
        te = te->next;
    }
    
    return nearest;
}

/* Process time events */
static int processTimeEvents(aeEventLoop *eventLoop) {
    int processed = 0;
    aeTimeEvent *te, *prev;
    long long now = aeGetTime();
    
    te = eventLoop->timeEventHead;
    prev = NULL;
    
    while(te) {
        long long when = te->when_sec * 1000 + te->when_ms;
        if (now >= when) {
            int retval;
            
            retval = te->timeProc(eventLoop, te->id, te->clientData);
            processed++;
            
            if (retval != AE_NOMORE) {
                te->when_sec = now / 1000 + retval / 1000;
                te->when_ms = now % 1000 + retval % 1000;
                if (te->when_ms >= 1000) {
                    te->when_sec++;
                    te->when_ms -= 1000;
                }
            } else {
                /* Delete this time event */
                if (prev == NULL)
                    eventLoop->timeEventHead = te->next;
                else
                    prev->next = te->next;
                    
                free(te);
                te = prev ? prev->next : eventLoop->timeEventHead;
                continue;
            }
        }
        
        prev = te;
        te = te->next;
    }
    
    return processed;
}

/* Process events */
int aeProcessEvents(aeEventLoop *eventLoop, int flags) {
    int processed = 0, numevents;
    
    /* Nothing to do? return ASAP */
    if (!(flags & AE_TIME_EVENTS) && !(flags & AE_FILE_EVENTS)) return 0;
    
    /* Note that we want to call aeApiPoll() even if there are no
     * file events to process as long as we want to process time
     * events, in order to sleep until the next time event is ready
     * to fire. */
    if (eventLoop->maxfd != -1 || ((flags & AE_TIME_EVENTS) && !(flags & AE_DONT_WAIT))) {
        int j;
        struct timeval tv, *tvp;
        aeTimeEvent *shortest = NULL;
        
        if (flags & AE_TIME_EVENTS && !(flags & AE_DONT_WAIT))
            shortest = aeSearchNearestTimer(eventLoop);
            
        if (shortest) {
            long long now = aeGetTime();
            long long ms = (shortest->when_sec * 1000 + shortest->when_ms) - now;
            
            if (ms > 0) {
                tv.tv_sec = ms / 1000;
                tv.tv_usec = (ms % 1000) * 1000;
            } else {
                tv.tv_sec = 0;
                tv.tv_usec = 0;
            }
            tvp = &tv;
        } else {
            /* If we have to check for events but need to return
             * ASAP because of AE_DONT_WAIT we need to set the timeout
             * to zero */
            if (flags & AE_DONT_WAIT) {
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                tvp = &tv;
            } else {
                /* Otherwise we can block */
                tvp = NULL;
            }
        }
        
        if (eventLoop->beforeSleep != NULL && flags & AE_CALL_BEFORE_SLEEP)
            eventLoop->beforeSleep(eventLoop);
            
        numevents = aeApiPoll(eventLoop, tvp);
        
        for (j = 0; j < numevents; j++) {
            aeFileEvent *fe = &eventLoop->events[eventLoop->fired[j].fd];
            int mask = eventLoop->fired[j].mask;
            int fd = eventLoop->fired[j].fd;
            
            /* Handle readable event */
            if (mask & AE_READABLE && fe->rfileProc) {
                fe->rfileProc(eventLoop, fd, fe->clientData, mask);
                processed++;
            }
            
            /* Handle writable event */
            if (mask & AE_WRITABLE && fe->wfileProc) {
                fe->wfileProc(eventLoop, fd, fe->clientData, mask);
                processed++;
            }
        }
    }
    
    /* Check time events */
    if (flags & AE_TIME_EVENTS)
        processed += processTimeEvents(eventLoop);
        
    return processed;
}

/* Wait for milliseconds until the given file descriptor becomes
 * writable/readable/exception */
int aeWait(int fd, int mask, long long milliseconds) {
    struct kevent ke;
    struct timespec ts, *tp;
    int retmask = 0, retval;
    
    if (milliseconds != -1) {
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        tp = &ts;
    } else {
        tp = NULL;
    }
    
    EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kqueue(), &ke, 1, NULL, 0, NULL) == -1) return -1;
    
    EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (kevent(kqueue(), &ke, 1, NULL, 0, NULL) == -1) return -1;
    
    struct kevent events[2];
    retval = kevent(kqueue(), NULL, 0, events, 2, tp);
    
    if (retval > 0) {
        if (events[0].filter == EVFILT_READ) retmask |= AE_READABLE;
        if (events[0].filter == EVFILT_WRITE) retmask |= AE_WRITABLE;
        if (retval > 1) {
            if (events[1].filter == EVFILT_READ) retmask |= AE_READABLE;
            if (events[1].filter == EVFILT_WRITE) retmask |= AE_WRITABLE;
        }
    }
    
    /* Cleanup */
    EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kqueue(), &ke, 1, NULL, 0, NULL);
    
    EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(kqueue(), &ke, 1, NULL, 0, NULL);
    
    return retmask;
}

/* Run the event loop */
void aeMain(aeEventLoop *eventLoop) {
    eventLoop->stop = 0;
    
    while (!eventLoop->stop) {
        if (eventLoop->beforeSleep != NULL)
            eventLoop->beforeSleep(eventLoop);
        aeProcessEvents(eventLoop, AE_ALL_EVENTS);
    }
}

/* Set before sleep proc */
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, void (*beforesleep)(aeEventLoop *eventLoop)) {
    eventLoop->beforeSleep = beforesleep;
}

/* Get set size */
int aeGetSetSize(aeEventLoop *eventLoop) {
    return eventLoop->setsize;
}

/* Resize set size */
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize) {
    if (setsize == eventLoop->setsize) return AE_OK;
    if (eventLoop->maxfd >= setsize) return AE_ERR;
    
    if (aeApiResize(eventLoop, setsize) == -1) return AE_ERR;
    
    eventLoop->events = realloc(eventLoop->events, sizeof(aeFileEvent) * setsize);
    eventLoop->fired = realloc(eventLoop->fired, sizeof(aeFiredEvent) * setsize);
    eventLoop->setsize = setsize;
    
    return AE_OK;
}
