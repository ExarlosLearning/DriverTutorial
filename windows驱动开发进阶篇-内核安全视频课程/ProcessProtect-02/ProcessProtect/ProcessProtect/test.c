#include <ntifs.h>
#include <ntddk.h>

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;//�����Ա��ϵͳ���м���(������ֹͣû��ж��)�Ѿ���ȡ���ڴ��� ���ǹ�ϵ��һ��  ����Ҫ�������� ˫���� �����м��ĸ��ڵ㶼���Ա����������� �������������������һ���ڵ�
	LIST_ENTRY InMemoryOrderLinks;//ϵͳ�Ѿ����� û�б���ʼ�� û�е���DriverEntry������̵�ʱ�� ͨ�����������̴�������
	LIST_ENTRY InInitializationOrderLinks;//�Ѿ�����DriverEntry���������������������
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
NTSTATUS EnableProtect(BOOLEAN Enable);
HANDLE ObHandle;
HANDLE ObHandle1;
PCHAR PsGetProcessImageFileName(IN PEPROCESS Process);
VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	EnableProtect(FALSE);
	KdPrint(("����жж\n"));
}
NTSTATUS BypassDriverCheck(PDRIVER_OBJECT DriverObject)
{
	if (DriverObject==NULL)
	{
		return STATUS_INVALID_PARAMETER;
	}
	PLDR_DATA_TABLE_ENTRY ldrData = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
	ldrData->Flags |= 0x20;
	return STATUS_SUCCESS;
}


OB_PREOP_CALLBACK_STATUS
Precb1(
_In_ PVOID RegistrationContext,
_Inout_ POB_PRE_OPERATION_INFORMATION OperationInformation
)
{

	PEPROCESS Process;
	PETHREAD Thread;
	if (OperationInformation->ObjectType != *PsThreadType)
	{
		goto End;
	}
	Thread = OperationInformation->Object;
	Process = IoThreadToProcess(Thread);
	if (_stricmp(PsGetProcessImageFileName(Process), "calc.exe") == 0)
	{
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)//
		{
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess&THREAD_TERMINATE)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE;
			}
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess&THREAD_SUSPEND_RESUME)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~THREAD_SUSPEND_RESUME;
			}


		}
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess&THREAD_TERMINATE)
			{
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE;
			}
			if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess&THREAD_SUSPEND_RESUME)
			{
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~THREAD_SUSPEND_RESUME;
			}
		}

	}



End:


	return OB_PREOP_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS
Precb(
_In_ PVOID RegistrationContext,
_Inout_ POB_PRE_OPERATION_INFORMATION OperationInformation
)
{
#define 	PROCESS_TERMINATE 0x0001 
	PEPROCESS Process;
	if (OperationInformation->ObjectType != *PsProcessType)
	{
		goto End;
	}
	Process = OperationInformation->Object;
	if (_stricmp(PsGetProcessImageFileName(Process),"calc.exe")==0)
	{
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess&PROCESS_TERMINATE)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
			
		}
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess&PROCESS_TERMINATE)
			{
				OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
		}
		
	}
	


End:


	return OB_PREOP_SUCCESS;
}

NTSTATUS EnableProtect(BOOLEAN Enable)
{
	NTSTATUS status = STATUS_SUCCESS;
	
	if (Enable)
	{
		OB_CALLBACK_REGISTRATION ObCb = { 0 };
		OB_OPERATION_REGISTRATION ObOp = { 0 };

		OB_CALLBACK_REGISTRATION ObCb1 = { 0 };
		OB_OPERATION_REGISTRATION ObOp1 = { 0 };

		ObCb.Version = OB_FLT_REGISTRATION_VERSION;
		ObCb.RegistrationContext = NULL;
		ObCb.OperationRegistrationCount = 1;
		ObCb.OperationRegistration = &ObOp;
		RtlInitUnicodeString(&ObCb.Altitude, L"123456");
		ObOp.ObjectType = PsProcessType;
		ObOp.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		ObOp.PreOperation = Precb;
		
		status = ObRegisterCallbacks(&ObCb, &ObHandle);
		if (NT_SUCCESS(status))
		{
			KdPrint(("ע��ɹ�\n"));
		}
		else
		{
			KdPrint(("���̻ص�ע��ʧ��%x\n", status));
		}


		ObCb1.Version = OB_FLT_REGISTRATION_VERSION;
		ObCb1.RegistrationContext = NULL;
		ObCb1.OperationRegistrationCount = 1;
		ObCb1.OperationRegistration = &ObOp1;
		RtlInitUnicodeString(&ObCb1.Altitude, L"32412");
		ObOp1.ObjectType = PsThreadType;
		ObOp1.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		ObOp1.PreOperation = Precb1;

		status = ObRegisterCallbacks(&ObCb1, &ObHandle1);
		if (NT_SUCCESS(status))
		{
			KdPrint(("�̳߳ɹ�\n"));
		}
		else
		{
			KdPrint(("�̻߳ص�ע��ʧ��%x\n", status));
		}




	}
	else
	{
		if (ObHandle != NULL)
		{
			ObUnRegisterCallbacks(ObHandle);
		}
		if (ObHandle1 != NULL)
		{
			ObUnRegisterCallbacks(ObHandle1);
		}
	}

	return status;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	KdPrint(("��������\n"));
	DriverObject->DriverUnload = Unload;
	BypassDriverCheck(DriverObject);
	EnableProtect(TRUE);
	return STATUS_SUCCESS;
}