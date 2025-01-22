#include "Response.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * render_static_file(char * fileName) {
	FILE* file = fopen(fileName, "r");

	if (file == NULL) {
		return NULL;
	}else {
		printf("%s does exist \n", fileName);
	}

	fseek(file, 0, SEEK_END); //fseek() -> 파일의 위치를 이동시킴. SEEK_END -> 파일의 끝으로 이동
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET); //SEEK_SET -> 파일의 시작으로 이동

	char* temp = malloc(sizeof(char) * (fsize+1)); //malloc으로 heap에서 size+1만큼의 메모리 할당. 스트림 역할.
	char ch;
	int i = 0;
	while((ch = fgetc(file)) != EOF) { //fgetc() -> 파일에서 문자 하나를 읽어옴.EOF is end of file.
		temp[i] = ch;
		i++;
	}
	fclose(file); //파일 닫기
	return temp; //스트림에 담은 내용 flush()
}
