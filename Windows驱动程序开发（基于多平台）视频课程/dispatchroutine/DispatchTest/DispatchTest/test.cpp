#include <windows.h>
#include <stdio.h>
#include <winioctl.h>

#define IOTEST CTL_CODE(FILE_DEVICE_UNKNOWN,0X800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define CTLBUFFERED CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define CTLDIRECT	CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_ANY_ACCESS)
#define CTLNEITHER	CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_NEITHER,FILE_ANY_ACCESS)
int main()
{
	
	HANDLE hDevice = CreateFile(TEXT("\\\\.\\HelloDDK"),
		GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("�豸��ʧ��%d\n",GetLastError());
		return -1;
	}
	DWORD dwRet;
	CHAR Buffer[1024] = { 0 };
	WriteFile(hDevice, "����һ�������û��������", strlen("����һ�������û��������"), &dwRet, NULL);
	printf("%p\n", Buffer);
	ReadFile(hDevice, Buffer, sizeof(Buffer), &dwRet, NULL);
	printf("���ص�����%s\n", Buffer);
	printf("��ȡ���ݵĸ���%d\n", dwRet);
	
	DWORD dwSize = GetFileSize(hDevice, NULL);
	printf("�ļ�����%d\n", dwSize);
	char IoBuffer[1024] = { 0 };
	DeviceIoControl(hDevice, CTLBUFFERED, "���ǻ�������ʽ", strlen("���ǻ�������ʽ") + 1, IoBuffer, sizeof(IoBuffer), &dwRet, NULL);
	printf("%s\n", IoBuffer);

	RtlZeroMemory(IoBuffer, sizeof(Buffer));
	DeviceIoControl(hDevice, CTLDIRECT, "����ֱ������ʽ", strlen("����ֱ������ʽ") + 1, IoBuffer, sizeof(IoBuffer), &dwRet, NULL);
	printf("%s\n", IoBuffer);

	RtlZeroMemory(IoBuffer, sizeof(Buffer));
	DeviceIoControl(hDevice, CTLNEITHER, "������������ʽ", strlen("������������ʽ") + 1, IoBuffer, sizeof(IoBuffer), &dwRet, NULL);
	printf("%s\n", IoBuffer);

	CloseHandle(hDevice);

	return 0;
}