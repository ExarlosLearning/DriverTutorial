#include <ntddk.h>
#include <windef.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

VOID RegCreateTest()
{
	HANDLE hKey;
	NTSTATUS status;
	OBJECT_ATTRIBUTES oa;
	ULONG ulRet;
	HANDLE hSubKey;
	UNICODE_STRING SubName=RTL_CONSTANT_STRING(L"MySubKey");
	//HKEY_LOCAL_MACHINE\SOFTWARE
	UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\MyKey");
	InitializeObjectAttributes(&oa, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateKey(&hKey, KEY_ALL_ACCESS, &oa, 0, NULL, REG_OPTION_NON_VOLATILE, &ulRet);
	if (NT_SUCCESS(status))
	{ 
		if (ulRet == REG_CREATED_NEW_KEY)
		{
			KdPrint(("����ڴ����ɹ�\n"));
		}
		else if (ulRet ==REG_OPENED_EXISTING_KEY)
		{
			KdPrint(("����ڣ�����\n"));
		}
	}
	else
	{
		KdPrint(("ʧ��1\n"));
	}


	InitializeObjectAttributes(&oa, &SubName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, hKey, NULL);
	status = ZwCreateKey(&hSubKey, KEY_ALL_ACCESS, &oa, 0, NULL, REG_OPTION_NON_VOLATILE, &ulRet);
	if (NT_SUCCESS(status))
	{
		if (ulRet == REG_CREATED_NEW_KEY)
		{
			KdPrint(("������ڴ����ɹ�\n"));
		}
		else if (ulRet == REG_OPENED_EXISTING_KEY)
		{
			KdPrint(("������ڣ�����\n"));
		}
	}
	else
	{
		KdPrint(("ʧ��2\n"));
	}


	ZwClose(hSubKey);
	ZwClose(hKey);


}

VOID RegOpenTest()
{
	HANDLE hKey;
	NTSTATUS status;
	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\MyKey");
	UNICODE_STRING ValueName;
	DWORD Value = -1;
	ULONG Length;
	CHAR Buffer[] = { "hello word" };
	PKEY_VALUE_PARTIAL_INFORMATION pvpi = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(PagedPool, 1024);
	InitializeObjectAttributes(&oa, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &oa);
	if (NT_SUCCESS(status))
	{
		KdPrint(("�򿪳ɹ�\n"));
	}
	else
	{
		KdPrint(("��ʧ��\n"));
	}

	RtlInitUnicodeString(&ValueName, L"�ַ���");
	ZwSetValueKey(hKey, &ValueName, 0, REG_SZ, L"���", wcslen(L"���")*sizeof(WCHAR));

	RtlInitUnicodeString(&ValueName, L"����");
	ZwSetValueKey(hKey, &ValueName, 0, REG_DWORD, &Value, sizeof(DWORD));

	RtlInitUnicodeString(&ValueName, L"������");
	ZwSetValueKey(hKey, &ValueName, 0, REG_BINARY, Buffer, strlen(Buffer));

	/*****************************��ȡ*******************************************/
	RtlZeroMemory(pvpi, 1024);
	RtlInitUnicodeString(&ValueName, L"�ַ���");
	ZwQueryValueKey(hKey, &ValueName, KeyValuePartialInformation, pvpi, 1024, &Length);
	if (pvpi->Type== REG_SZ)
	{
		KdPrint(("%ls\n", pvpi->Data));
	}

	RtlZeroMemory(pvpi, 1024);
	RtlInitUnicodeString(&ValueName, L"����");
	ZwQueryValueKey(hKey, &ValueName, KeyValuePartialInformation, pvpi, 1024, &Length);
	if (pvpi->Type == REG_DWORD)
	{
		KdPrint(("%d\n", *(PULONG)pvpi->Data));
	}

	RtlZeroMemory(pvpi, 1024);
	RtlInitUnicodeString(&ValueName, L"������");
	ZwQueryValueKey(hKey, &ValueName, KeyValuePartialInformation, pvpi, 1024, &Length);
	if (pvpi->Type == REG_BINARY)
	{
		KdPrint(("%s\n", pvpi->Data));
	}



	ZwClose(hKey);


}

VOID RegEnumTest()
{
	HANDLE hKey;
	NTSTATUS status;
	OBJECT_ATTRIBUTES oa;
	//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control
	ULONG Length;
	ULONG Index;
	UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\System\\CurrentControlSet\\Control");
	PKEY_FULL_INFORMATION pfi = (PKEY_FULL_INFORMATION)ExAllocatePool(PagedPool, 1024);
	PKEY_BASIC_INFORMATION pbi = (PKEY_BASIC_INFORMATION)ExAllocatePool(PagedPool, 1024);
	PKEY_VALUE_FULL_INFORMATION pvpi = (PKEY_VALUE_FULL_INFORMATION)ExAllocatePool(PagedPool, 1024);
	InitializeObjectAttributes(&oa, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &oa);
	if (!NT_SUCCESS(status))
	{
		return;
	}
	status = ZwQueryKey(hKey, KeyFullInformation, pfi, 1024, &Length);
	if (NT_SUCCESS(status))
	{
		KdPrint(("****************************************\n"));
		for (Index = 0; Index < pfi->SubKeys;Index++)
		{
			RtlZeroMemory(pbi, 1024);
			status = ZwEnumerateKey(hKey, Index, KeyBasicInformation, pbi, 1024, &Length);
			if (NT_SUCCESS(status))
			{
				KdPrint(("%ls\n", pbi->Name));
			}
		}
		KdPrint(("****************************************\n"));
		for (Index = 0; Index < pfi->Values;Index++)
		{
			RtlZeroMemory(pvpi, 1024);
			status = ZwEnumerateValueKey(hKey, Index, KeyValueFullInformation, pvpi, 1024, &Length);
			if (NT_SUCCESS(status))
			{
				switch (pvpi->Type)
				{
				case REG_DWORD:
					KdPrint(("%ls:%d\n", pvpi->Name, *(PULONG)((PCHAR)pvpi + pvpi->DataOffset)));
					break;
				case REG_SZ:
				case REG_MULTI_SZ:
				case REG_EXPAND_SZ:
					KdPrint(("%ls:%ls\n", pvpi->Name, (PCHAR)pvpi + pvpi->DataOffset));
					break;
				default:

					break;
				}
			}


		}

	}

	ZwClose(hKey);
	if (pfi != NULL)
	{
		ExFreePool(pfi);
	}
	if (pbi!=NULL)
	{
		ExFreePool(pbi);
	}
	if (pvpi!=NULL)
	{
		ExFreePool(pvpi);
	}



}

VOID RegDeleteTest()
{
	HANDLE hKey;
	NTSTATUS status;
	OBJECT_ATTRIBUTES oa;
	//HKEY_LOCAL_MACHINE\SOFTWARE\mykey\mysubkey
	UNICODE_STRING RegPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\mykey\\mysubkey");
	UNICODE_STRING RegPath1 = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\SOFTWARE\\mykey");
	UNICODE_STRING ValueName;
	InitializeObjectAttributes(&oa, &RegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &oa);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("ע�������\n"));
		return;
	}
	ZwDeleteKey(hKey);
	ZwClose(hKey);
	InitializeObjectAttributes(&oa, &RegPath1, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &oa);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("ע�������\n"));
		return;
	}
	RtlInitUnicodeString(&ValueName, L"����");
	ZwDeleteValueKey(hKey, &ValueName);

	RtlInitUnicodeString(&ValueName, L"�ַ���");
	ZwDeleteValueKey(hKey, &ValueName);
	ZwClose(hKey);

}

VOID RtlTest()
{
	NTSTATUS status;
	status = RtlCreateRegistryKey(RTL_REGISTRY_SERVICES, L"Myservice");
	if (NT_SUCCESS(status))
	{
		KdPrint(("Rtl��������ɹ�\n"));
	}
	status = RtlCheckRegistryKey(RTL_REGISTRY_SERVICES, L"MyService");
	if (NT_SUCCESS(status))
	{
		KdPrint(("ע������\n"));
	}
	status = RtlWriteRegistryValue(RTL_REGISTRY_SERVICES, L"MyService", L"�ַ���", REG_SZ, L"�������", 9);
	if (NT_SUCCESS(status))
	{
		KdPrint(("д��ɹ�\n"));
	}

	status = RtlDeleteRegistryValue(RTL_REGISTRY_SERVICES, L"MyService", L"�ַ���");
	if (NT_SUCCESS(status))
	{
		KdPrint(("ɾ���ɹ�\n"));
	}

}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;

//	RegCreateTest();
// 	RegOpenTest();
// 	RegEnumTest();
//	RegDeleteTest();

	RtlTest();

	return STATUS_SUCCESS;
}