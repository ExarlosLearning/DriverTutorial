#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	
	KdPrint(("����ж��\n"));
	
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload=Unload;
	
	return STATUS_SUCCESS;
}