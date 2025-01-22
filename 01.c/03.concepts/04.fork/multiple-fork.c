#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int N = 5;

    pid_t pid[N]; /* process id */
    int childStatus;
    int i;

    // Create multiple child processes
    for (i = 0; i < N; i++)
    {
        pid[i] = fork(); //부모 프로세스가 5번의 fork를 통해 pid배열에 저장하게 된다.
        //이때 컨텍스트 스위칭(Context switching)을 통해 부모 프로세스가 아닌 자식 프로세스가 시작된다면 자식 프로세스가 진행되고,
        //부모 프로세스가 아닌 자식 프로세스가 시작되지 않았다면 부모 프로세스가 진행된다.

        if (pid[i] == 0)
        {
            printf("Now pid[%d] has died\n",i);
            exit(100+i);
        }
    }

    for (i = 0; i < N; i++)
    {
        pid_t terminatedChild = wait(&childStatus); //부모 프로세스는 자식 프로세스가 종료되기 기다림.

        // The child process has termindated normally
        if (WIFEXITED(childStatus))
        {
            printf("Child %d has terminated with exit status %d\n", terminatedChild, WEXITSTATUS(childStatus));
        }
        else
            printf("Child %d has terminated abnormally\n", terminatedChild);
    }
    printf("Parent process has terminated\n");
}


//console.log


/* Now pid[0] has died */
/* Now pid[1] has died */
/* Now pid[2] has died */
/* Child 8336 has terminated with exit status 102 */
/* Child 8335 has terminated with exit status 101 */
/* Child 8334 has terminated with exit status 100 */
/* Now pid[3] has died */
/* Now pid[4] has died */
/* Child 8338 has terminated with exit status 104 */
/* Child 8337 has terminated with exit status 103 */
/* Parent process has terminated */



