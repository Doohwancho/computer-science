#include <stdio.h>
#define MAX_INPUT 120
#define FILENAME "input.txt"

int main()
{
	//r�� read
	//w�� ���� �ԷµȰ� �� ������
	//a�� ���� �Էµ� ���� ������ ���ο� ���� �߰�

	/*
	//���� ���� ����
	int a, b, result;
	FILE* fp; //��Ʈ��(������ ����Ű�� ������)
	
	printf("������ �� �� ���� �Է��Ͻÿ� : ");
	scanf_s("%d %d", &a, &b);
	result = a * b;

	if (fopen_s(&fp, FILENAME, "wt"))
	{
		printf("���� ���� ����\n");
		return 0;
	}

	fputs("������ ����� ���ڽ��ϴ�.\n",fp);
	fprintf(fp,"ù��°��: %d\n", a);
	fprintf(fp,"�ι�°��: %d\n", b);
	fprintf(fp,"�������: %d\n", result);

	fclose(fp);
	return 0;
	*/
	/*
	������ ����� ���ڽ��ϴ�.
	ù��°��: 3
	�ι�°��: 5
	�������: 15
	*/
	

	/*
	//���� �Է� ����

	errno_t err;
	FILE* fp; //��Ʈ��(������ ����Ű�� ������)
	int a, b, result;
	char text[MAX_INPUT];

	if (fopen_s(&fp, FILENAME, "rt"))
	{
		printf("���� ���� ����\n");
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

	//����3

	FILE* fp;
	if (fopen_s(&fp, "step33FileIO.cpp", "rt"))
	{
		printf("���� ���� ����\n");
		return 0;
	}
	char ch;
	//while (!feof(fp)) //end of file�� �ƴϸ� true
	//{jj
	//	fscanf_s(fp, "%c", &ch, sizeof(ch)); //�� ���Ͼ� ������ �ѱ��ھ� �޾Ƽ�
	//	printf("%c", ch); //���
	//}
	while(fscanf_s(fp, "%c", &ch, sizeof(ch)) != EOF) printf("%c", ch);

	fclose(fp);
}