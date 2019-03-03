#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

VOID ThreadProc(PVOID Context)
{
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * 2000 * 1000);
	PKSEMAPHORE pSemaphore = (PKSEMAPHORE)Context;
	KdPrint(("����ϵͳ�̺߳���\n"));
	KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	KeReleaseSemaphore(pSemaphore, IO_NO_INCREMENT, 1, FALSE);
	KdPrint(("�뿪ϵͳ�̺߳���\n"));
	PsTerminateSystemThread(0);


}

VOID SemaphoreTest()
{
	KSEMAPHORE Semaphore;
	LONG Count;
	HANDLE hThread;
	KeInitializeSemaphore(&Semaphore, 2, 2);
	Count = KeReadStateSemaphore(&Semaphore);
	KdPrint(("%d\n", Count));

	KeWaitForSingleObject(&Semaphore, Executive, KernelMode, FALSE, NULL);
	Count = KeReadStateSemaphore(&Semaphore);
	KdPrint(("%d\n", Count));

	KeWaitForSingleObject(&Semaphore, Executive, KernelMode, FALSE, NULL);
	Count = KeReadStateSemaphore(&Semaphore);
	KdPrint(("%d\n", Count));


	PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, &Semaphore);

	KeWaitForSingleObject(&Semaphore, Executive, KernelMode, FALSE, NULL);
	KdPrint(("����\n"));



}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	SemaphoreTest();
	return STATUS_SUCCESS;
}