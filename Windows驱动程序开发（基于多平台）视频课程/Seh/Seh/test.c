#include <ntddk.h>

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("����ж��\n"));
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING RegistryPath)
{
	PCHAR Buffer = NULL;
	LONG f1, f2, f3;
	DriverObject->DriverUnload = Unload;
	KdPrint(("��������\n"));

// 	__try
// 	{
// 		//KdPrint(("%d\n", *Buffer));
// 		//ProbeForRead(Buffer, 4, 4);
// 		//ProbeForWrite(Buffer, 4, 4);
// 		//ExRaiseStatus(STATUS_ACCESS_VIOLATION);
// 		//ExRaiseStatus(STATUS_DATATYPE_MISALIGNMENT);
// 		//ExRaiseAccessViolation();
// 		ExRaiseDatatypeMisalignment();
// 	}
// 	__except (EXCEPTION_EXECUTE_HANDLER)
// 	{
// 		KdPrint(("�쳣��%x\n", GetExceptionCode()));
// 	}
	f1 = 0;
	f2 = 1;
	f3 = 2;
	__try
	{
		if (f1 == 0)
		{
			KdPrint(("��f1\n"));
			__leave;
		}

		if (f2 == 1)
		{
			KdPrint(("��f2\n"));
			__leave;
		}

		if (f3 == 2)
		{
			KdPrint(("��f3\n"));
			__leave;
		}
	
	}
	__finally
	{
		KdPrint(("��finally\n"));
	}

	KdPrint(("DriverEntry����\n"));
	
	return STATUS_SUCCESS;
}