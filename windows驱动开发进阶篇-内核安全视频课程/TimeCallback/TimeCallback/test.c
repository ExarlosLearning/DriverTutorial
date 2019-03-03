#include <ntddk.h>

HANDLE hCallback;

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	if (hCallback != NULL)
	{
		ExUnregisterCallback(hCallback);
	}
	KdPrint(("����ж��\n"));
}

//DISPATCH_LEVEL
VOID
callbackroutine(
IN PVOID CallbackContext,
IN PVOID Argument1,
IN PVOID Argument2
)
{
	KdPrint(("%d:ʱ�䷢�����޸�\n",KeGetCurrentIrql()));
	

}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING CbName = RTL_CONSTANT_STRING(L"\\Callback\\SetSystemTime");
	OBJECT_ATTRIBUTES oa;
	NTSTATUS status;
	PCALLBACK_OBJECT CallBack;

	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	InitializeObjectAttributes(&oa, &CbName, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ExCreateCallback(&CallBack, &oa, TRUE, FALSE);
	if (NT_SUCCESS(status))
	{
		KdPrint(("�ɹ�\n"));
		hCallback = ExRegisterCallback(CallBack, callbackroutine, NULL);
		ObDereferenceObject(CallBack);
	}
	else
	{
		KdPrint(("ʧ��\n"));
	}

	return STATUS_SUCCESS;
}