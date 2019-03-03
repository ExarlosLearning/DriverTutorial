#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\Global??\\HelloDDK");
	IoDeleteSymbolicLink(&SymbolicLinkName);
	IoDeleteDevice(DriverObject->DeviceObject);
	KdPrint(("����ж��\n"));

}

VOID EnumDriver(PDRIVER_OBJECT DriverObjet)
{

	KdPrint(("Driver:%p\n", DriverObjet));
	KdPrint(("Device:%p\n", DriverObjet->DeviceObject));
	KdPrint(("����������:%wZ\n", &DriverObjet->DriverName));
	KdPrint(("������:%wZ\n", &DriverObjet->DriverExtension->ServiceKeyName));
	KdPrint(("%wZ\n", DriverObjet->HardwareDatabase));
	

}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloDDK");
	UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloDDK");
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸����ʧ��\n"));
		return status;
	}
	KdPrint(("�豸�����ɹ�\n"));
	status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("������������ʧ��%x\n", status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

	KdPrint(("�������Ӵ����ɹ�\n"));
	DeviceObject->Flags |= DO_BUFFERED_IO;
	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;


	EnumDriver(DriverObject);




	return STATUS_SUCCESS;
}

