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

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	EnumDriver(DriverObject);



	return STATUS_SUCCESS;
}