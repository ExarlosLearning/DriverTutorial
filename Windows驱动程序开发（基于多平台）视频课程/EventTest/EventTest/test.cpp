#include <windows.h>
#include <stdio.h>

DWORD WINAPI Threadproc(PVOID Context)
{
	PHANDLE phEvent = (PHANDLE)Context;
	printf("���뵽�̺߳�����\n");
	SetEvent(*phEvent);
	printf("�̺߳�������\n");

	return 0;
}

int main()
{
	HANDLE hEvent=CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == hEvent)
	{
		printf("�����¼�ʧ��%d\n", GetLastError());
		return -1;
	}

	CreateThread(NULL, 0, Threadproc, &hEvent, 0, NULL);

	WaitForSingleObject(hEvent, INFINITE);
	printf("���̽���\n");
	return 0;
}