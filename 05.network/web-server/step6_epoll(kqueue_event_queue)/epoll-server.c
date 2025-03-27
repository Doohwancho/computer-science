// Asynchronous socket server - accepting multiple clients concurrently,
// multiplexing the connections with kqueue (macOS/BSD) instead of epoll.
//
// Eli Bendersky [http://eli.thegreenplace.net]
// This code is in the public domain.
// Modified for macOS/kqueue

//part1) 헤더 파일 및 기본 설정:
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h> //mac m1은 arm64라 kqueue를 써야함
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define MAXFDS 16 * 1024
#define MAX_EVENTS 64  // Maximum number of events to process in one kqueue call

//part2) 상태 관리를 위한 구조체 및 열거형
typedef enum { INITIAL_ACK, WAIT_FOR_MSG, IN_MSG } ProcessingState;

#define SENDBUF_SIZE 1024

typedef struct {
  ProcessingState state;
  uint8_t sendbuf[SENDBUF_SIZE];
  int sendbuf_end;
  int sendptr;
} peer_state_t;

// Each peer is globally identified by the file descriptor (fd) it's connected
// on. As long as the peer is connected, the fd is unique to it. When a peer
// disconnects, a new peer may connect and get the same fd. on_peer_connected
// should initialize the state properly to remove any trace of the old peer on
// the same fd.
peer_state_t global_state[MAXFDS];


//part3) 파일 디스크립터 상태 관리를 위한 구조체

// Callbacks (on_XXX functions) return this status to the main loop; the status
// instructs the loop about the next steps for the fd for which the callback was
// invoked.
// want_read=true means we want to keep monitoring this fd for reading.
// want_write=true means we want to keep monitoring this fd for writing.
// When both are false it means the fd is no longer needed and can be closed.
typedef struct {
  bool want_read;
  bool want_write;
} fd_status_t;

// These constants make creating fd_status_t values less verbose.
const fd_status_t fd_status_R = {.want_read = true, .want_write = false};
const fd_status_t fd_status_W = {.want_read = false, .want_write = true};
const fd_status_t fd_status_RW = {.want_read = true, .want_write = true};
const fd_status_t fd_status_NORW = {.want_read = false, .want_write = false};


//part4) 콜백 함수들:
// on_peer_connected: 새 클라이언트 연결 시 호출
// on_peer_ready_recv: 클라이언트에서 데이터를 받을 준비가 되었을 때 호출
// on_peer_ready_send: 클라이언트에게 데이터를 보낼 준비가 되었을 때 호출

fd_status_t on_peer_connected(int sockfd, const struct sockaddr_in* peer_addr,
                              socklen_t peer_addr_len) {
  assert(sockfd < MAXFDS);
  report_peer_connected(peer_addr, peer_addr_len);

  // Initialize state to send back a '*' to the peer immediately.
  peer_state_t* peerstate = &global_state[sockfd];
  peerstate->state = INITIAL_ACK;
  peerstate->sendbuf[0] = '*';
  peerstate->sendptr = 0;
  peerstate->sendbuf_end = 1;

  // Signal that this socket is ready for writing now.
  return fd_status_W;
}

fd_status_t on_peer_ready_recv(int sockfd) {
  assert(sockfd < MAXFDS);
  peer_state_t* peerstate = &global_state[sockfd];

  if (peerstate->state == INITIAL_ACK ||
      peerstate->sendptr < peerstate->sendbuf_end) {
    // Until the initial ACK has been sent to the peer, there's nothing we
    // want to receive. Also, wait until all data staged for sending is sent to
    // receive more data.
    return fd_status_W;
  }

  uint8_t buf[1024];
  int nbytes = recv(sockfd, buf, sizeof buf, 0);
  if (nbytes == 0) {
    // The peer disconnected.
    return fd_status_NORW;
  } else if (nbytes < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // The socket is not *really* ready for recv; wait until it is.
      return fd_status_R;
    } else {
      perror_die("recv");
    }
  }
  bool ready_to_send = false;
  for (int i = 0; i < nbytes; ++i) {
    switch (peerstate->state) {
    case INITIAL_ACK:
      assert(0 && "can't reach here");
      break;
    case WAIT_FOR_MSG:
      if (buf[i] == '^') {
        peerstate->state = IN_MSG;
      }
      break;
    case IN_MSG:
      if (buf[i] == '$') {
        peerstate->state = WAIT_FOR_MSG;
      } else {
        assert(peerstate->sendbuf_end < SENDBUF_SIZE);
        peerstate->sendbuf[peerstate->sendbuf_end++] = buf[i] + 1;
        ready_to_send = true;
      }
      break;
    }
  }
  // Report reading readiness iff there's nothing to send to the peer as a
  // result of the latest recv.
  return (fd_status_t){.want_read = !ready_to_send,
                       .want_write = ready_to_send};
}

fd_status_t on_peer_ready_send(int sockfd) {
  assert(sockfd < MAXFDS);
  peer_state_t* peerstate = &global_state[sockfd];

  if (peerstate->sendptr >= peerstate->sendbuf_end) {
    // Nothing to send.
    return fd_status_RW;
  }
  int sendlen = peerstate->sendbuf_end - peerstate->sendptr;
  int nsent = send(sockfd, &peerstate->sendbuf[peerstate->sendptr], sendlen, 0);
  if (nsent == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return fd_status_W;
    } else {
      perror_die("send");
    }
  }
  if (nsent < sendlen) {
    peerstate->sendptr += nsent;
    return fd_status_W;
  } else {
    // Everything was sent successfully; reset the send queue.
    peerstate->sendptr = 0;
    peerstate->sendbuf_end = 0;

    // Special-case state transition in if we were in INITIAL_ACK until now.
    if (peerstate->state == INITIAL_ACK) {
      peerstate->state = WAIT_FOR_MSG;
    }

    return fd_status_R;
  }
}

// Helper function to update kqueue for a socket.
// This replaces the epoll_ctl calls with the equivalent kqueue operations.
void update_fd_in_kqueue(int kq, int fd, fd_status_t status) {
  struct kevent kevs[2];
  int nchanges = 0;
  
  // Clear any existing events first
  EV_SET(&kevs[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  EV_SET(&kevs[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
  
  // Ignore errors from the delete operations as the events might not be registered yet
  kevent(kq, kevs, 2, NULL, 0, NULL);
  
  // Reset for adding the new events
  nchanges = 0;
  
  // Add read filter if requested
  if (status.want_read) {
    EV_SET(&kevs[nchanges++], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  }
  
  // Add write filter if requested
  if (status.want_write) {
    EV_SET(&kevs[nchanges++], fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
  }
  
  // Apply the changes
  if (nchanges > 0) {
    if (kevent(kq, kevs, nchanges, NULL, 0, NULL) < 0) {
      perror_die("kevent register");
    }
  }
}

// part5) 메인함수
// kqueue 인스턴스 생성
// 리스닝 소켓 설정 및 kqueue에 등록
// 이벤트 루프에서 kevent 호출하여 이벤트 처리

int main(int argc, const char** argv) {
  setvbuf(stdout, NULL, _IONBF, 0);

  int portnum = 8080;
  if (argc >= 2) {
    portnum = atoi(argv[1]);
  }
  printf("Serving on port %d\n", portnum);

  int listener_sockfd = listen_inet_socket(portnum);
  make_socket_non_blocking(listener_sockfd);

  // Create kqueue instance
  int kq = kqueue();
  if (kq < 0) {
    perror_die("kqueue");
  }

  // Register the listening socket with kqueue for read events
  struct kevent kev;
  EV_SET(&kev, listener_sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0) {
    perror_die("kevent register listener");
  }

  // Allocate memory for events
  struct kevent* events = (struct kevent*)calloc(MAX_EVENTS, sizeof(struct kevent));
  if (events == NULL) {
    die("Unable to allocate memory for kevent events");
  }

  while (1) {
    // Wait for events indefinitely
    int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
    if (nev < 0) {
      perror_die("kevent wait");
    }

    for (int i = 0; i < nev; i++) {
      int fd = events[i].ident;
      
      // Check for errors
      if (events[i].flags & EV_ERROR) {
        printf("Error on fd %d: %s\n", fd, strerror(events[i].data));
        close(fd);
        continue;
      }

      if (fd == listener_sockfd) {
        // The listening socket is ready; this means a new peer is connecting.
        struct sockaddr_in peer_addr;
        socklen_t peer_addr_len = sizeof(peer_addr);
        int newsockfd = accept(listener_sockfd, (struct sockaddr*)&peer_addr,
                              &peer_addr_len);
        
        if (newsockfd < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // This can happen due to the nonblocking socket mode; in this
            // case don't do anything, but print a notice (since these events
            // are extremely rare and interesting to observe...)
            printf("accept returned EAGAIN or EWOULDBLOCK\n");
          } else {
            perror_die("accept");
          }
        } else {
          make_socket_non_blocking(newsockfd);
          if (newsockfd >= MAXFDS) {
            die("socket fd (%d) >= MAXFDS (%d)", newsockfd, MAXFDS);
          }

          fd_status_t status =
              on_peer_connected(newsockfd, &peer_addr, peer_addr_len);
          
          // Register the new socket with kqueue
          update_fd_in_kqueue(kq, newsockfd, status);
        }
      } else {
        // A peer socket is ready
        fd_status_t status;
        
        if (events[i].filter == EVFILT_READ) {
          // Ready for reading
          status = on_peer_ready_recv(fd);
        } else if (events[i].filter == EVFILT_WRITE) {
          // Ready for writing
          status = on_peer_ready_send(fd);
        } else {
          // This shouldn't happen
          continue;
        }
        
        if (status.want_read || status.want_write) {
          // Update the kqueue registrations for this socket
          update_fd_in_kqueue(kq, fd, status);
        } else {
          // The socket is no longer needed, close it
          printf("socket %d closing\n", fd);
          close(fd);
        }
      }
    }
  }

  return 0;
}
