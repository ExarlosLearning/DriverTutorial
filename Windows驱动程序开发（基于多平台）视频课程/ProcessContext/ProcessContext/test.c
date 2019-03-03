#include <ntddk.h>

PCHAR PsGetProcessImageFileName(IN PEPROCESS Process);

typedef struct _MYDATA 
{
	LIST_ENTRY ListEntry;
	int number;
}MYDATA,*PMYDATA;

typedef struct _MYDATA2
{
	int number;
}MYDATA2, *PMYDATA2;

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	//KdPrint(("%s\n", PsGetProcessImageFileName(PsGetCurrentProcess())));
	KdPrint(("����ж��\n"));

}

VOID LinkListTest()
{
	LIST_ENTRY ListHead;
	InitializeListHead(&ListHead);
	KdPrint(("�������\n"));
	for (int i = 0; i < 10; i++)
	{
		PMYDATA pdata = (PMYDATA)ExAllocatePool(PagedPool, sizeof(MYDATA));
		pdata->number = i;
		InsertHeadList(&ListHead, &pdata->ListEntry);
	}

	while (!IsListEmpty(&ListHead))
	{
		PLIST_ENTRY pEntry = RemoveTailList(&ListHead);
		PMYDATA pdata = CONTAINING_RECORD(pEntry, MYDATA, ListEntry);
		KdPrint(("%d\n", pdata->number));
		ExFreePool(pdata);
	}



}

VOID LookAsideTest()
{
	PMYDATA2 data[50] = { 0 };
	int i;
	PAGED_LOOKASIDE_LIST LookAsideList;
	ExInitializePagedLookasideList(&LookAsideList, NULL, NULL, 0, sizeof(MYDATA2), 'abcd', 0);;
	;

	for (i = 0; i < 50;i++)
	{
		data[i] = ExAllocateFromPagedLookasideList(&LookAsideList);
		data[i]->number = i;
	}

	for (i = 0; i < 50;i++)
	{
		KdPrint(("%d\n", data[i]->number));
		ExFreeToPagedLookasideList(&LookAsideList,data[i]);
	}
	ExDeletePagedLookasideList(&LookAsideList);

}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	PCHAR pcstr;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	pcstr = (PCHAR)ExAllocatePool(NonPagedPool, 1024);
	if (pcstr == NULL)
	{
		KdPrint(("�ڴ����ʧ��\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	KdPrint(("�ڴ����ɹ�\n"));
	RtlZeroMemory(pcstr, 1024);
	strcpy(pcstr, "����һ���ڴ����Ĳ���");
	KdPrint(("%s\n", pcstr));
	ExFreePool(pcstr);

	pcstr = (PCHAR)ExAllocatePoolWithTag(PagedPool, 1024, 'abcd');
	if (pcstr == NULL)
	{
		KdPrint(("�ڴ����ʧ��\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	KdPrint(("����ǩ���ڴ����ɹ�\n"));
	RtlZeroMemory(pcstr, 1024);
	strcpy(pcstr, "����һ�δ���ǩ���ڴ����\n");
	KdPrint(("%s\n", pcstr));
	ExFreePoolWithTag(pcstr,'abcd');

	//KdPrint(("%s\n", PsGetProcessImageFileName(PsGetCurrentProcess())));
	LinkListTest();
	LookAsideTest();
	return STATUS_SUCCESS;
}