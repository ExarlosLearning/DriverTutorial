#include <stdio.h>

struct MYST 
{
	int *p;
	int a[4];
};

int test()
{
	printf("����ָ�����\n");

	return 0;
}

int main()
{
	struct MYST st[100];

	printf("%d", sizeof(st));


	return 0;
}