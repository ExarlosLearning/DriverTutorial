#include <ntddk.h>
#include <ntstrsafe.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING us = RTL_CONSTANT_STRING(L"Hello world!");
	ANSI_STRING as = RTL_CONSTANT_STRING("Hello world!");
	UNICODE_STRING usDest;
	ANSI_STRING asDest;
	ULONG Value;
	LONG ret;
	WCHAR wcstr[1024] = { 0 };
	CHAR asstr[1024] = { 0 };
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	KdPrint(("%wZ\n", &us));
	KdPrint(("%Z\n", &as));
	RtlInitUnicodeString(&us, L"��ã�����");
	RtlInitAnsiString(&as, "���,����");


	KdPrint(("%wZ\n", &us));
	KdPrint(("%Z\n", &as));
	

	RtlInitEmptyUnicodeString(&usDest, wcstr, sizeof(wcstr));
	RtlInitEmptyAnsiString(&asDest, asstr, sizeof(asstr));

	RtlCopyUnicodeString(&usDest, &us);
	RtlCopyString(&asDest, &as);

	KdPrint(("%wZ\n", &usDest));
	KdPrint(("%Z\n", &asDest));
	RtlInitUnicodeString(&us, L"��ã���");
	ret = RtlCompareUnicodeString(&us, &usDest, TRUE);
	if (ret == 0)
	{
		KdPrint(("us=usDest\n"));
	}
	else if (ret < 0)
	{
		KdPrint(("us<usDest\n"));
	}
	else
	{
		KdPrint(("us>usDest\n"));
	}

	RtlInitAnsiString(&as, "��ã�����\n");

	ret = RtlCompareString(&as, &asDest, TRUE);
	if (ret==0)
	{
		KdPrint(("as=asDest\n"));
	}
	else if (ret > 0)
	{
		KdPrint(("as>asDest\n"));
	}
	else
	{
		KdPrint(("as<asDest\n"));
	}


	RtlInitUnicodeString(&us, L"abcDf����");
	RtlUpcaseUnicodeString(&usDest, &us, FALSE);
	KdPrint(("%wZ\n", &usDest));

	RtlInitAnsiString(&as, "abcdEf");
	RtlUpperString(&asDest, &as);
	KdPrint(("%Z\n", &asDest));
	

	RtlInitUnicodeString(&us, L"-123");
	RtlUnicodeStringToInteger(&us, 10, &Value);
	KdPrint(("%d\n", Value));

	RtlIntegerToUnicodeString(123, 10, &usDest);
	KdPrint(("%wZ\n", &usDest));

	RtlInitUnicodeString(&us, L"����unicodestring");
	RtlUnicodeStringToAnsiString(&asDest, &us, FALSE);

	KdPrint(("%Z\n", &asDest));

	RtlInitAnsiString(&as, "����ansistring");
	RtlAnsiStringToUnicodeString(&usDest, &as, FALSE);
	KdPrint(("%wZ\n", &usDest));
	
	RtlInitUnicodeString(&us, L"����");
	//10����������ansistringhelloworld
	RtlUnicodeStringPrintf(&usDest, L"%d��%wZ%Z%s%S", 10, &us,&asDest, L"hello", "world");
	KdPrint(("%wZ\n", &usDest));
	return STATUS_SUCCESS;
}