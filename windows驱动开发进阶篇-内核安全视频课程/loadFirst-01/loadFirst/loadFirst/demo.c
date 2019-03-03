#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{


	KdPrint(("����ж��\n"));
}

VOID ThreadProc(IN PVOID Context)
{
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * 1000 * 1000);
	while (TRUE)
	{
		KdPrint(("������������...\n"));
		KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	}
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	HANDLE hThread;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, NULL);
	return STATUS_SUCCESS;
}