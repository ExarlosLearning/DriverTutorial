#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{


	KdPrint(("����ж��\n"));
}

VOID ThreadProc(IN PVOID Context)
{
	LARGE_INTEGER timeout = RtlConvertLongToLargeInteger(-10 * 1000 * 1000);
	while (TRUE)
	{
		KdPrint(("������������...\n"));
		KeDelayExecutionThread(KernelMode, FALSE, &timeout);
	}
}

VOID Reinitialize(
	_In_     struct _DRIVER_OBJECT *DriverObject,
	_In_opt_ PVOID                 Context,
	_In_     ULONG                 Count
	)
{
	KdPrint(("�������³�ʼ�����\n"));
	if (Count < 10)
	{
		IoRegisterDriverReinitialization(DriverObject, Reinitialize, NULL);
	}
	else
	{
		KdPrint(("���³�ʼ����������\n"));
	}

	


}


VOID BootReinitialize(
	_In_     struct _DRIVER_OBJECT *DriverObject,
	_In_opt_ PVOID                 Context,
	_In_     ULONG                 Count
	)
{
	KdBreakPoint();
	KdPrint(("boot�������³�ʼ������\n"));
//	IoRegisterBootDriverReinitialization(DriverObject, BootReinitialize, NULL);
}

NTSTATUS lfShutDown(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	KdBreakPoint();
	KdPrint(("�ػ�֪ͨ\n"));

	//�ػ�����������,����ָ��ļ���ˢ�´����ļ��ȣ��޸�ע����
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	HANDLE hThread;
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS status;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	//PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, NULL, NULL, NULL, ThreadProc, NULL);

	DriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = lfShutDown;
	status = IoCreateDevice(DriverObject, 0, NULL, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸����ʧ��%x\n",status));
	}
	IoRegisterShutdownNotification(DeviceObject);
// 	IoRegisterDriverReinitialization(DriverObject, Reinitialize, NULL);
// 	IoRegisterBootDriverReinitialization(DriverObject, BootReinitialize, NULL);

	//IoRegisterShutdownNotification()

	return STATUS_SUCCESS;
}