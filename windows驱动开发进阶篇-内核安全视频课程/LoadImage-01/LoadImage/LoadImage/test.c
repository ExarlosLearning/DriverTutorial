#include <ntifs.h>
#include <ntddk.h>

PCHAR PsGetProcessImageFileName(IN PEPROCESS Process);

VOID
LoadImageNotify(
_In_ PUNICODE_STRING FullImageName,
_In_ HANDLE ProcessId,                // pid into which image is being mapped
_In_ PIMAGE_INFO ImageInfo
);

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	PsRemoveLoadImageNotifyRoutine(LoadImageNotify);
	KdPrint(("����ж��\n"));
}

VOID
LoadImageNotify(
_In_ PUNICODE_STRING FullImageName,
_In_ HANDLE ProcessId,                // pid into which image is being mapped
_In_ PIMAGE_INFO ImageInfo
)
{
	PEPROCESS Process;
	
	if (ProcessId == 0)
	{
		KdPrint(("ģ�����%wZ\n", FullImageName));
	}
	else
	{
		PsLookupProcessByProcessId(ProcessId, &Process);
		KdPrint(("%d:%s,%wZ\n",ProcessId, PsGetProcessImageFileName(Process), FullImageName));
	}

}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	PsSetLoadImageNotifyRoutine(LoadImageNotify);

	return STATUS_SUCCESS;
}