#include <stdio.h>
#define MAX_INPUT 120
#define FILENAME "input.txt"

int main()
{
	//r은 read
	//w은 원래 입력된게 다 지워짐
	//a는 원래 입력된 내용 다음에 새로운 내용 추가

	/*
	//파일 쓰기 예제
	int a, b, result;
	FILE* fp; //스트림(파일을 가리키는 포인터)
	
	printf("곱셈을 할 두 수를 입력하시오 : ");
	scanf_s("%d %d", &a, &b);
	result = a * b;

	if (fopen_s(&fp, FILENAME, "wt"))
	{
		printf("파일 열기 실패\n");
		return 0;
	}

	fputs("곱셈의 결과를 보겠습니다.\n",fp);
	fprintf(fp,"첫번째수: %d\n", a);
	fprintf(fp,"두번째수: %d\n", b);
	fprintf(fp,"곱셈결과: %d\n", result);

	fclose(fp);
	return 0;
	*/
	/*
	곱셈의 결과를 보겠습니다.
	첫번째수: 3
	두번째수: 5
	곱셈결과: 15
	*/
	

	/*
	//파일 입력 예제

	errno_t err;
	FILE* fp; //스트림(파일을 가리키는 포인터)
	int a, b, result;
	char text[MAX_INPUT];

	if (fopen_s(&fp, FILENAME, "rt"))
	{
		printf("파일 열기 실패\n");
		return 0;
	}
	
	fgets(text, MAX_INPUT, fp);
	printf("%s", text);
	fscanf_s(fp, "%s %d", text, sizeof(text), &a);
	printf("%s %d\n", text, a);
	fscanf_s(fp, "%s %d", text, sizeof(text), &b);
	printf("%s %d\n", text, b);
	fscanf_s(fp, "%s %d", text, sizeof(text), &result);
	printf("%s %d\n", text, result);


	fclose(fp);
	return 0;
	*/

	//예제3

	FILE* fp;
	if (fopen_s(&fp, "step33FileIO.cpp", "rt"))
	{
		printf("파일 열기 실패\n");
		return 0;
	}
	char ch;
	//while (!feof(fp)) //end of file이 아니면 true
	//{jj
	//	fscanf_s(fp, "%c", &ch, sizeof(ch)); //이 파일안 정보를 한글자씩 받아서
	//	printf("%c", ch); //출력
	//}
	while(fscanf_s(fp, "%c", &ch, sizeof(ch)) != EOF) printf("%c", ch);

	fclose(fp);
}