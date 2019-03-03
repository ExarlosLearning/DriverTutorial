#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{


	KdPrint(("����ж��\n"));
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	LARGE_INTEGER systemtime;
	LARGE_INTEGER localtime;
	TIME_FIELDS timefields;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	KeQuerySystemTime(&systemtime);
	ExSystemTimeToLocalTime(&systemtime, &localtime);
	RtlTimeToTimeFields(&localtime, &timefields);

	KdPrint(("%d��%d��%d�� %d:%d:%d.%.3d\n", 
		timefields.Year, 
		timefields.Month, 
		timefields.Day, 
		timefields.Hour, 
		timefields.Minute, 
		timefields.Second,
		timefields.Milliseconds));


	return STATUS_SUCCESS;
}