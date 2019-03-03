#include <ntddk.h>

typedef struct _KSYSTEM_SERVICE_TABLE
{
	PULONG  ServiceTableBase;                                       // ��������ַ���ַ  
	PULONG  ServiceCounterTableBase;
	ULONG   NumberOfService;                                        // �������ĸ���  
	PULONG   ParamTableBase;                                        // �������������ַ   
} KSYSTEM_SERVICE_TABLE, *PKSYSTEM_SERVICE_TABLE;

#ifdef _X86_
extern PKSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;
#else
#endif

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{

	KdPrint(("����ж��\n"));
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	ULONG i = 0;
	for (i = 0; i < KeServiceDescriptorTable->NumberOfService; i++)
	{

		KdPrint(("%d\t0x%p\n", i,KeServiceDescriptorTable->ServiceTableBase[i]));
	}
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}