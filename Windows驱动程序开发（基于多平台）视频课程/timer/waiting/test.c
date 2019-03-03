#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{


	KdPrint(("����ж��\n"));
}

VOID WaitTest1(ULONG ulMicroSeconds)
{
	KEVENT Event;
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * ulMicroSeconds);
	KeInitializeEvent(&Event, SynchronizationEvent, FALSE);
	KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, &timeout);

	KdPrint(("�ȴ�����1\n"));
}

VOID WaitTest2(ULONG ulMicroSeconds)
{
	KTIMER timer;
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * ulMicroSeconds);
	KeInitializeTimer(&timer);

	KeSetTimer(&timer, timeout, NULL);
	KeWaitForSingleObject(&timer, Executive, KernelMode, FALSE, &timeout);
	KdPrint(("�ȴ�����2\n"));
}

VOID WaitTest3(ULONG ulMicroSeconds)
{
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * ulMicroSeconds);
	KeDelayExecutionThread(KernelMode, FALSE, &timeout);

	KdPrint(("�ȴ�����3\n"));
}

VOID WaitTest4(ULONG ulMicroSeconds)
{

	KeStallExecutionProcessor(ulMicroSeconds);


	KdPrint(("�ȴ�����4\n"));
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	WaitTest1(2000*1000);
	WaitTest2(2000 * 1000);
	WaitTest3(2000 * 1000);
	WaitTest4(50);
	return STATUS_SUCCESS;
}