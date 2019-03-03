#include <ntddk.h>

PCHAR PsGetProcessImageFileName(IN PEPROCESS Process);

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

VOID ThreadProc(PVOID Context)
{
	KdPrint(("%s\n", PsGetProcessImageFileName(PsGetCurrentProcess())));

	PsTerminateSystemThread(0); 
}

VOID CreateThreadTest()
{
	HANDLE hThread;
	HANDLE hProcess;
	NTSTATUS status;
	CLIENT_ID ClientId = { NULL };
	OBJECT_ATTRIBUTES oa;

	PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, NULL);

	ClientId.UniqueProcess = (HANDLE)1424;
	InitializeObjectAttributes(&oa, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &oa, &ClientId);
	if (NT_SUCCESS(status))
	{
		PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, NULL, ThreadProc, NULL);
	}
	KdPrint(("����\n"));

}

VOID ThreadProc1(PVOID Context)
{
	PKEVENT pEvent = (PKEVENT)Context;
	KdPrint(("�����߳�1\n"));
	KeSetEvent(pEvent, IO_NO_INCREMENT, FALSE);

	KdPrint(("�뿪�߳�1\n"));
	PsTerminateSystemThread(0);
	

}

VOID EventTest()
{
	KEVENT Event;
	HANDLE hThread;

	KeInitializeEvent(&Event, NotificationEvent, FALSE);

	PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc1, &Event);

	KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);

	KdPrint(("���߳̽���\n"));


}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	PKEVENT  pEvent;
	HANDLE hEvent;
	UNICODE_STRING EventName = RTL_CONSTANT_STRING(L"\\BaseNamedObjects\\����¼�");
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	pEvent = IoCreateSynchronizationEvent(&EventName, &hEvent);
	pEvent = IoCreateNotificationEvent(&EventName, &hEvent);
	if (pEvent == NULL)
	{
		KdPrint(("����ʧ��\n"));
	}
	KdPrint(("�����ɹ�\n"));
	ZwClose(hEvent);
//	CreateThreadTest();
//	EventTest();

	return STATUS_SUCCESS;
}