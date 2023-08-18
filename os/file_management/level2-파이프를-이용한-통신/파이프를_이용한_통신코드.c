#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


//파이프로 두 프로세스가 통신하는 코드
int main()
{
    int pid, fd[2];
    char buf[5];


    if(pipe(fd) == -1) {
        exit(-1);
    }

    pid = fork();

    if(pid < 0) {
        perror("Error forking");
        exit(-1);
    }
    else if(pid==0) {            // child
        close(fd[0]); //unused
        write(fd[1], "Test", 5);
        close(fd[1]);
        exit(0);
    } else {                     // parent process
        close(fd[1]); //unused
        read(fd[1], buf, 5);
        close(fd[0]);
        printf("%s", buf);
        exit(0);
    }
    return 0;
}
