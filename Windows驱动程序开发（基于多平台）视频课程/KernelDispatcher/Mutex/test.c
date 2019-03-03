#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

VOID ThreadProc1(PVOID Context)
{
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * 2000 * 1000);
	PKMUTEX pMutex = (PKMUTEX)Context;
	KeWaitForMutexObject(pMutex, Executive, KernelMode, FALSE, NULL);
	KdPrint(("�����߳�1\n"));

	KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	KdPrint(("�뿪�߳�1\n"));
	KeReleaseMutex(pMutex, FALSE);
	
	PsTerminateSystemThread(0);

}

VOID ThreadProc2(PVOID Context)
{

	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * 2000 * 1000);
	PKMUTEX pMutex = (PKMUTEX)Context;
	KeWaitForMutexObject(pMutex, Executive, KernelMode, FALSE, NULL);
	KdPrint(("�����߳�2\n"));

	KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	KdPrint(("�뿪�߳�2\n"));
	KeReleaseMutex(pMutex, FALSE);

	PsTerminateSystemThread(0);

}

VOID MutexTest()
{
	KMUTEX Mutex;
	HANDLE hThread[2] = { NULL };
	PETHREAD Thread[2] = { NULL };
	KeInitializeMutex(&Mutex, 0);

	PsCreateSystemThread(&hThread[0], THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc1, &Mutex);
	PsCreateSystemThread(&hThread[1], THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc2, &Mutex);

	ObReferenceObjectByHandle(hThread[0], THREAD_ALL_ACCESS, *PsThreadType, KernelMode, &Thread[0], NULL);
	ObReferenceObjectByHandle(hThread[1], THREAD_ALL_ACCESS, *PsThreadType, KernelMode, &Thread[1], NULL);

	KeWaitForMultipleObjects(2, Thread, WaitAll, Executive, KernelMode, FALSE, NULL, NULL);


	ObDereferenceObject(Thread[0]);
	ObDereferenceObject(Thread[1]);
	KdPrint(("���߳̽���\n"));
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	MutexTest();
	return STATUS_SUCCESS;
}