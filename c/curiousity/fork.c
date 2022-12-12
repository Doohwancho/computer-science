#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int a = 6; /* global (external) variable */


int main(void)
{
    int b; /* local variable */
    int childStatus;
    pid_t pid; /* process id */


    b = 88;

    printf("Before fork\n");

    pid = fork(); //부모 process에 pc, fd, stack, heap, data 영역 복사해서 자식 process 생성. return 값은 부모 process에겐 자식 프로세스의 pid. 가 들어가고, 자식 프로세스에겐 0이 들어감.
    printf("This is pid value :: %d // %d\n",pid,getpid()); //부모 프로세스가 print한 것.
    if (pid == 0)
    { /* child */
        a++; b++;
        printf("This is the child and the test number is %d %d // %d\n",a,b,getpid());
    }

    else /* parent */
    {
        wait(&childStatus); //자식 프로세스가 끝날 때 까지 기다려줌. 자식 프로세스 종료시 상태정보를 가져옴.
        printf("This is the parent and the test number is %d %d // %d\n",a,b,getpid());
    }

    return 0;
}

//console.log

/* Before fork */
/* /1* This is pid value :: 1040 // 1039 *1/ */
/* This is pid value :: 0 // 1040 */
/* This is the child and the test number is 7 89 // 1040 */
/* This is the parent and the test number is 6 88 // 1039 */


// what is fork()?

// fork() is a system call that creates a new process, which is called the child process, which runs concurrently with the process that makes the fork() call (which is called the parent process).


