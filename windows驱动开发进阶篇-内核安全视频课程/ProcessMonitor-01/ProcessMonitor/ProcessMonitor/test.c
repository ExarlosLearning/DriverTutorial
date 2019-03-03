#include <ntifs.h>
#include <ntddk.h>

VOID
ProcessNotify(
IN HANDLE  ParentId,
IN HANDLE  ProcessId,
IN BOOLEAN  Create
);

PCHAR PsGetProcessImageFileName(PEPROCESS Process);
VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	PsSetCreateProcessNotifyRoutine(ProcessNotify, TRUE);
}


VOID
ProcessNotify(
IN HANDLE  ParentId,
IN HANDLE  ProcessId,
IN BOOLEAN  Create
)
{
	PEPROCESS Process;
	PsLookupProcessByProcessId(ProcessId, &Process);
	if (Create)
	{
		
		KdPrint(("�н��̴���%d:%s:%d\n",ParentId, PsGetProcessImageFileName(Process),ProcessId));
		
	}
	else
	{
		KdPrint(("�н����˳�%s:%d\n",PsGetProcessImageFileName(Process),ProcessId));
	}

}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	PsSetCreateProcessNotifyRoutine(ProcessNotify, FALSE);

	DriverObject->DriverUnload = Unload;
	return STATUS_SUCCESS;
}