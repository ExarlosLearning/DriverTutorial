#include <ntddk.h>


typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;//�����Ľ���� DriverEntry
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;//��������·��
	UNICODE_STRING BaseDllName;//����·������������
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemModuleInformation = 11
}SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
	HANDLE Section;
	PVOID MappedBase;
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT PathLength;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Count;
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;



NTSTATUS ZwQuerySystemInformation(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength
	);



VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	
	KdPrint(("����ж��\n"));

}


VOID EnumDriver(PDRIVER_OBJECT DriverObject)
{
	PLDR_DATA_TABLE_ENTRY pDataTable = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
	PLIST_ENTRY ListHeader = &pDataTable->InLoadOrderLinks;
	PLIST_ENTRY ListEntry;
	ListEntry = ListHeader;
	KdPrint(("������\t����ַ\t��С\t��������\t����·��\t\t\t��������\n"));
	do 
	{
		pDataTable = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		KdPrint(("%wZ\t\t\t0x%p\\t\t\t0x%x\t\t\t0x%p\t\t\t%wZ\n",
			&pDataTable->BaseDllName,
			pDataTable->DllBase,
			pDataTable->SizeOfImage,
			DriverObject,
			&pDataTable->FullDllName
			));
		ListEntry = ListEntry->Flink;
	} while (ListEntry!=ListHeader);
		
}

VOID EnumDriver2()
{
	NTSTATUS status;
	KdPrint(("����\n"));
	PSYSTEM_MODULE_INFORMATION psysmodule = (PSYSTEM_MODULE_INFORMATION)ExAllocatePool(PagedPool, 1024 * 1024);
	ULONG i = 0;
	ULONG ret;
	if (psysmodule != NULL)
	{
		RtlZeroMemory(psysmodule, 1024 * 1024);

		status = ZwQuerySystemInformation(SystemModuleInformation, psysmodule, 1024 * 1024, &ret);
		if (NT_SUCCESS(status))
		{
			for (i = 0; i < psysmodule->Count;i++)
			{
				KdPrint(("%s\t%p:0x%x\n", psysmodule->Module[i].ImageName,
					psysmodule->Module[i].Base,
					psysmodule->Module[i].Size));
			}
		}
		else
		{
			KdPrint(("ģ���ѯʧ��%x\n", status));
		}

	}
	

	



}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	//EnumDriver(DriverObject);
	EnumDriver2();
	

	return STATUS_SUCCESS;
}