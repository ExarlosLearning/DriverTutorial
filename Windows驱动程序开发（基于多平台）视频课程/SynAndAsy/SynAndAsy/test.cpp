#include <windows.h>
#include <stdio.h>

VOID CALLBACK FileIOCompletionRoutine(
	_In_    DWORD        dwErrorCode,
	_In_    DWORD        dwNumberOfBytesTransfered,
	_Inout_ LPOVERLAPPED lpOverlapped
	)
{

	printf("�Ѿ�����˶�����\n");


}

int Syn()
{
	HANDLE hFile = CreateFile(TEXT("test.txt"),
		GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ���ʧ��%d\n", GetLastError());
		return -1;
	}
	DWORD dwret;
	OVERLAPPED ol = { NULL };
	ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WriteFile(hFile, "�����첽����", strlen("�����첽����"), &dwret, &ol);



	WaitForSingleObject(ol.hEvent, INFINITE);
	char Buffer[1024] = { 0 };
	RtlZeroMemory(&ol, sizeof(ol));
	ReadFileEx(hFile, Buffer, sizeof(Buffer), &ol, FileIOCompletionRoutine);
	SleepEx(INFINITE, TRUE);
	printf("%s\n", Buffer);
	printf("����\n");

	CloseHandle(hFile);

	return 0;
}

int main()
{
	
	Syn();

	return 0;
}