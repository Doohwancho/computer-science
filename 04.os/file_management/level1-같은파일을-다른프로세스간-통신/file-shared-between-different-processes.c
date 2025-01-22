#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//부모 프로세스와 자식 프로세스가 통신하는 코드
int main()
{
    int pid, fd;
    char buf[5];

    fd=open("com.txt", O_RDWR);  // init

    if(fd < 0) {
        perror("Error opening file");
        exit(-1);
    }

    pid = fork();

    if(pid < 0) {
        perror("Error forking");
        exit(-1);
    }
    else if(pid==0) {            // child
        write(fd, "Test", 5); // Write 5 characters (without null terminator)
        close(fd);
        exit(0);
    } else {                     // parent process
        lseek(fd, 0, SEEK_SET);
        read(fd, buf, 5); // Read 5 characters
        printf("%s", buf);
        close(fd);
        exit(0);
    }
    return 0;
}
