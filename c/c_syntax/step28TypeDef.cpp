#include <stdio.h>

//type define = ���� ����


int main()
{
	//unsigned short int age;

	typedef int int32;
	int32 n = 20; //int�� ���Ʈ���� �˷��ֱ� ���� int32�ιٲ�

	printf("%d\n", n);
	printf("%d\n", sizeof(int32)); //4 == int�� ����

	typedef int Pair[2]; //2��¥�� int�� �迭 ����
	Pair point = { 3,4 }; // int point[2] = {3,4};
	//���� ��ǥ��� ����

	printf("(%d, %d)\n", point[0], point[1]);

	/*
	<����>

	typedef char *String;

	String name = "Doohwan";

	�� �����ڵ�
	error C2440:  '�ʱ�ȭ ��': 'const char [8]'���� 'String'(��)�� ��ȯ�� �� �����ϴ�.


	<����>

	"C������ ���ڿ� ���ͷ��� char�� �迭������, C++������ const char�� �迭�̴�."
	��, char *�� ���ڿ� ���ͷ��� ��� ����� C ��Ÿ���̸�, C++������ �߸��� �ڵ�����̱� ������ C2440 ������ �߻��ϴ� ���̴�.


	<�ذ���>

	1. pointer�� �ƴ� array �������� ����

	char s1[] = "aa";


	2. (char *) �������� ����ȯ(casting)

	char *s1 = (char*)"aa";


	3. �ڷ����� const char* ���·� �ٲٱ�

	const char *s1 = "aa";


	4. const_cast<char*> ����ϱ�

	char *s1 = const_cast<char*>("aa");
	*/

	typedef char String;

	String name1[] = "Doohwan"; // char *name = "Doohwan";

	printf("�̸�: %s\n", name1);

	typedef const char *Stringtwo;

	Stringtwo name2 = "Daehwan"; // char *name = "Doohwan";

	printf("�̸�: %s\n", name2);

}