#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadProc(PVOID Context)
{
	PHANDLE phSemaphore = (PHANDLE)Context;
	printf("�����̺߳���\n");
	Sleep(5000);
	ReleaseSemaphore(*phSemaphore, 1, NULL);

	printf("�뿪�̺߳���\n");
	return 0;


}

int main()
{
	HANDLE hSemaphore = CreateSemaphore(NULL, 2, 2, NULL);
	if (NULL == hSemaphore)
	{
		printf("�ź�������ʧ��%d\n", GetLastError());
		return -1;
	}

	WaitForSingleObject(hSemaphore, INFINITE);
	WaitForSingleObject(hSemaphore, INFINITE);

	CreateThread(NULL, 0, ThreadProc, &hSemaphore, 0, NULL);

	WaitForSingleObject(hSemaphore, INFINITE);
	printf("���߳̽���\n");
	return 0;
}