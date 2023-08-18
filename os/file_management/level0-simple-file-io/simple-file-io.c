#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
   ---
   1. com.txt에 적혀있는 것

   Test
   wax
   end


   ---
   2. 실제로 파일에 저장되있는 것

   Test\nwax\nend\0   (한줄로, \n로 엔터, \0가 마지막으로 저장되어있다)
*/

int main()
{
    int fd;
    char buf[5];

    fd=open("com.txt", O_RDWR); //Test\nwax\nend\0 에서 fd는 index0인 'T'를 가르킨다.
    read(fd, buf, 5); //['T','e','s','t','\0'] 가 저장됨.(4+맨 마지막 '\0' +1까지)
    printf("%s", buf);
    //만약 이 상태에서 쓰기연산하면, 5번 인덱스부터 써진다..
    close(fd);
    return 0;
}
