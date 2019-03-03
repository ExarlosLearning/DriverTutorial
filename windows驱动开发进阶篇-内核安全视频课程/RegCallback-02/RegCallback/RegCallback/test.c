#include <ntddk.h>

LARGE_INTEGER Cookie;
WCHAR KeyPath[1024] = { 0 };
WCHAR KeyName[128] = { 0 };

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	CmUnRegisterCallback(Cookie);
	KdPrint(("����ж��\n"));
}

NTSTATUS RegistryCallback(
	_In_      PVOID CallbackContext,
	_In_opt_  PVOID Argument1,
	_In_opt_  PVOID Argument2
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PREG_CREATE_KEY_INFORMATION pOpenKeyinfo;
	//KdPrint(("��ע���Ĳ���\n"));

	switch ((REG_NOTIFY_CLASS)Argument1)
	{
	case RegNtPreOpenKey:
	case RegNtPreOpenKeyEx:
	case RegNtPreCreateKey:
	case RegNtPreCreateKeyEx:
		pOpenKeyinfo = (PREG_CREATE_KEY_INFORMATION)Argument2;
		
		RtlCopyMemory(KeyPath, pOpenKeyinfo->CompleteName->Buffer, pOpenKeyinfo->CompleteName->Length);
		if (wcsstr(_wcslwr(KeyPath), _wcslwr(KeyName)) != NULL)
		{
			status = STATUS_STACK_OVERFLOW;
		}
		break;
	default:
		break;
	}


	return status;

}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	RtlCopyMemory(KeyName, DriverObject->DriverExtension->ServiceKeyName.Buffer, DriverObject->DriverExtension->ServiceKeyName.Length);

	status = CmRegisterCallback(RegistryCallback, NULL, &Cookie);
	if (NT_SUCCESS(status))
	{
		KdPrint(("ע��ɹ�\n"));
		return status;
	}

	


	return status;
}