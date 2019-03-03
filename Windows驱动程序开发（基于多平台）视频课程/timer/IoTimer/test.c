#include <ntddk.h>

KDPC dpc;
KTIMER timer;

LARGE_INTEGER timeout;

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloDDK");
	
	//IoStopTimer(DriverObject->DeviceObject);
	KeCancelTimer(&timer);
	IoDeleteDevice(DriverObject->DeviceObject);
	IoDeleteSymbolicLink(&SymbolicLinkName);
	KdPrint(("����ж��\n"));

}
void IoTimerRoutine(
	PDEVICE_OBJECT DeviceObject,
	PVOID Context
	)
{
	KdPrint(("io��ʱ��\n"));
}



VOID DpcRoutine(
	_In_     struct _KDPC *Dpc,
	_In_opt_ PVOID        DeferredContext,
	_In_opt_ PVOID        SystemArgument1,
	_In_opt_ PVOID        SystemArgument2
	)
{
	PIRP Irp = (PIRP)DeferredContext;
	KdPrint(("����dpc��ʱ����\n"));
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	//KeSetTimer(&timer, timeout, &dpc);
}

NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


NTSTATUS DispatchRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	timeout = RtlConvertLongToLargeInteger(-10 * 2000 * 1000);
	IoMarkIrpPending(Irp);

	
	KeInitializeTimer(&timer);
	KeInitializeDpc(&dpc, DpcRoutine, Irp);

	KeSetTimer(&timer, timeout, &dpc);

	return STATUS_PENDING;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloDDK");
	UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloDDK");
	NTSTATUS status;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸����ʧ��%d\n", status));
		return status;
	}

	status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�������Ӵ���ʧ��%d\n",status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

// 	IoInitializeTimer(DeviceObject, IoTimerRoutine, NULL);
// 	IoStartTimer(DeviceObject);
	
	
	return STATUS_SUCCESS;
}