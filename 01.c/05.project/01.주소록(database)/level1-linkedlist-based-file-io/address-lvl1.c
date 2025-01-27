#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//macos m1 compatible code
//source: https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=lsszz210&logNo=220282477592


struct List { //크기가 100인 배열로 이름,주소,번호,학번 변수 선언
    char name[100];
    char address[100];
    char tel[100];
    char number[100];
};

int main() {
    int a, b, c, d, e, g, h, i, j;
    //a: main 화면 input
    //b: case1: print() 에서 File *list 다 읽을 때 까지 ++됨. (\0)을 가르킴
    //c: case1: print() 에서 data[c] print할 때 쓰임. b-1.
    //d: case2: search()에서 선택지 고르는 입력값 용도로 쓰임
    //e: b+f (data의 index)
    int f = 0;
    //f: case3: Add()할 떄마다 +1.
    //g: case4: Update()에서 수정해야하는 data[g]의 인덱스
    //h: case4: Update()에서 수정할거 골랐으면, name | address | tel | number중 어느거 바꿀지 골라 할 때 쓰임
    struct List data[1000]; //구조체의 크기. buffered IO?! linkedlist 기반이네?
    char str[10]; //case4: Update()에서 수정할 이름. case5: Delete() 삭제할 사람의 이름?
    FILE *list; //파일의 list 포인터

    while (1) {
        printf("---------------------------주소록------------------------\n\n");
        printf("   ① 주소록 보기\n     \n");
        printf("   ② 주소록 검색\n     \n");
        printf("   ③ 주소록 추가\n     \n");
        printf("   ④ 주소록 수정\n     \n");
        printf("   ⑤ 주소록 삭제\n     \n");
        printf("   ⑥ 종       료\n     \n");
        printf("------------------------------------------------------------\n");
        scanf("%d", &a);

        switch (a) {
            case 1: //print all
                system("clear");
                list = fopen("ecomsw.txt", "r");
                if(list == NULL) {
                    printf("ecomsw.txt 파일을 찾을 수 없습니다.\n");
                    break;
                }
                for (b = 0;; b++) {
                    fscanf(list, "%s %s %s %s", &data[b].name, &data[b].number, &data[b].tel, &data[b].address); //fscanf = file + scanf. data에 있던 주솟값 넘겨서 list(File)에 쓴다.
                    if (feof(list)) { //int feof(FILE * stream) 이 함수는 파일의 끝에 도달했는지 여부를 확인할 때 사용하게 됩니다. 만약, 파일의 끝에 도달하게 되면 0이 아닌 값을 반환합니다. 즉 파일의 끝에 도달하지 못했을 경우에 0을 반환하는 것입니다.
                        break;
                    }
                }
                for (c = 0; c < b; c++) {
                    printf("%s  %s  %s  %s\n", data[c].name, data[c].number, data[c].tel, data[c].address);
                }
                fclose(list); //항상 메모리를 반환해줘야한다.. c에서는..
                break;

            case 2: //search
                system("clear");
                list = fopen("ecomsw.txt", "r");
                if(list == NULL) {
                    printf("ecomsw.txt 파일을 찾을 수 없습니다.\n");
                    break;
                }
                printf("① 이름으로 검색하기^▽^\n");
                printf("② 학번으로 검색하기^0^\n");
                scanf("%d", &d);
                switch (d) {
                    case 1:
                        printf("이름을 입력하세여!!");
                        scanf("%s", str);
                        for (c = 0; c < b; c++) {
                            if (strncmp(data[c].name, str, 6) == 0) {
                                printf("__________________________________________\n");
                                printf("이름 : %s\n학번 : %s\n번호 : %s\n주소 : %s\n",
                                    data[c].name, data[c].number, data[c].tel, data[c].address);
                                printf("__________________________________________\n");
                            }
                        }
                        break;
                    case 2:
                        printf("학번을 입력하세여!!");
                        scanf("%s", str);
                        for (c = 0; c < b; c++) {
                            if (strncmp(data[c].number, str, 7) == 0) {
                                printf("__________________________________________\n");
                                printf("이름 : %s\n학번 : %s\n번호 : %s\n주소 : %s\n",
                                    data[c].name, data[c].number, data[c].tel, data[c].address);
                                printf("__________________________________________\n");
                            }
                        }
                        break;
                }
                fclose(list); //항상 메모리를 반환해줘야한다.. c에서는..
                break;

            case 3: //add
                e = b + f; //b: case1에서 File *list 다 읽을 때 까지 ++됨. e는 data[index]
                list = fopen("ecomsw.txt", "ab+"); //ab+는 write를 의미하는듯?

                printf("이름 : ");
                scanf("%s", data[e].name);
                printf("학번 : ");
                scanf("%s", data[e].number);
                printf("번호 : ");
                scanf("%s", data[e].tel);
                printf("주소 : ");
                scanf("%s", data[e].address);

                gets(str); //input을 받아 char str[6]에 저장함

                fprintf(list, "%s %s %s %s\n", data[e].name, data[e].number, data[e].tel, data[e].address);
                printf("☆추카추카 입력성공!!☆\n");
                fclose(list); //항상 메모리를 반환해줘야한다.. c에서는..
                f++; //f: add할 때마다 +1.
                break;

            case 4: //update
                printf("수정할 이름을 입력하세여!!");
                scanf("%s", str);
                for (i = 0; i < b; i++) { //0~b(data에 쓰여진 index까지) 읽고
                    if (strncmp(data[i].name, str, 6) == 0) { //이름이 같다면?
                        g = i; //g에 수정할 data[g]의 인덱스를 넣음
                    }
                }
                printf("수정해볼께여!!\n\n1:이름\n2:학번\n3:번호\n4:주소\n");
                scanf("%d", &h);
                switch (h) {
                    case 1:
                        printf("이름: ");
                        scanf("%s", &data[g].name); //수정은 data[g]의 포인터를 넘겨서 원본수정
                        printf("☆수정완료☆\n");
                        break;
                    case 2:
                        printf("학번: ");
                        scanf("%s", &data[g].number);
                        printf("☆수정완료☆\n");
                        break;
                    case 3:
                        printf("번호: ");
                        scanf("%s", &data[g].tel);
                        printf("☆수정완료☆\n");
                        break;
                    case 4:
                        printf("주소: ");
                        scanf("%s", &data[g].address);
                        printf("☆수정완료☆.\n");
                        break;
                }
                //error: 여기에 gets(str); 같은거 없어서 data에만 반영됬지, 파일에는 반영 안될듯? -> WRONG!
                //data[g].address가 아니라, &data[g].address여서, 원래 파일도 바뀌는듯?
                list = fopen("ecomsw.txt", "w");
                if(list == NULL) {
                    printf("ecomsw.txt 파일을 찾을 수 없습니다.\n");
                    break;
                }
                for (c = 0; c < b; c++) {
                    fprintf(list, "%s %s %s %s\n", data[c].name, data[c].number, data[c].tel, data[c].address);
                }
                fclose(list); //메모리 반환해줘~
                break;

            case 5: //delete
                printf("삭제하실분 이름을 입력하세여!!");
                scanf("%s", &str);
                for (i = 0; i < b; i++) {
                    if (strncmp(data[i].name, str, 6) == 0) {
                        for (j = i + 1; j <= b; j++) {
                            strcpy(data[j - 1].name, data[j].name);
                            strcpy(data[j - 1].number, data[j].number);
                            strcpy(data[j - 1].tel, data[j].tel);
                            strcpy(data[j - 1].address, data[j].address);
                        }
                        b--;
                    }

                }
                list = fopen("ecomsw.txt", "w");
                for (c = 0; c < b; c++) {
                    fprintf(list, "%s %s %s %s\n", data[c].name, data[c].number, data[c].tel, data[c].address);
                }
                fclose(list); //메모리 반환해줘~
                break;

            case 6:
                exit(0);

        }
    }

    return 0;
}

