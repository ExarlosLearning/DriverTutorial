#include <windows.h>
#include <stdio.h>


DWORD WINAPI ThreadProc1(PVOID Context)
{
	PHANDLE phMutex = (PHANDLE)Context;
	WaitForSingleObject(*phMutex, INFINITE);
	printf("�����߳�1\n");
	Sleep(2000);
	printf("�뿪�߳�1\n"); 
	ReleaseMutex(*phMutex);
	return 0;
}

DWORD WINAPI ThreadProc2(PVOID Context)
{
	PHANDLE phMutex = (PHANDLE)Context;
	WaitForSingleObject(*phMutex, INFINITE);
	printf("�����߳�2\n");
	Sleep(2000);
	printf("�뿪�߳�2\n");
	ReleaseMutex(*phMutex);
	return 0;
}
int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL)
	{
		printf("�����¼�ʧ��%d\n",GetLastError());
		return -1;
	}
	HANDLE hThread[2] = { NULL };
	hThread[0] = CreateThread(NULL, 0, ThreadProc1, &hMutex, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ThreadProc2, &hMutex, 0, NULL);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	printf("�������߳�\n");
	return 0;
}