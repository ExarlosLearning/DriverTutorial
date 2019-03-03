#include <ntddk.h>

void* __cdecl operator new(size_t size)
{
	KdPrint(("����ȫ�ֵ�new\n"));
	return ExAllocatePool(PagedPool, size);
}

void __cdecl operator delete(void *p)
{
	KdPrint(("����ȫ�ֵ�delete\n"));
	if (p != NULL)
	{
		ExFreePool(p);
	}
}

class Myclass
{
public:
	Myclass()
	{
		KdPrint(("���캯��\n"));
	}

	~Myclass()
	{
		KdPrint(("��������\n"));
	}
	void* __cdecl operator new(size_t size, POOL_TYPE PoolType = PagedPool)
	{
		KdPrint(("�������е�new\n"));
		return ExAllocatePool(PoolType, size);
	}

	void __cdecl operator delete(void *p)
	{
		KdPrint(("�������е�delete\n"));
		if (p!=NULL)
		{
			ExFreePool(p);
		}
	}
	
protected:
private:
};

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{

}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	char Buffer[1024] = { 0 };
	char Src[128] = "�л����񹲺͹�";
	char Dst[1024] = "�л����񹲺͹�";
 	DriverObject->DriverUnload = Unload;
// 	PCHAR pstr = new char[100];
// 	delete[] pstr;
// 
// 	Myclass *pmyclass = new Myclass;
// 
// 	delete pmyclass;
	NTSTATUS status;
	RtlCopyMemory(Buffer, "����һ���ڴ�ĸ��Ʋ���", strlen("����һ���ڴ�ĸ��Ʋ���"));
	KdPrint(("%s\n", Buffer));
	RtlMoveMemory(Buffer, "����һ���ڴ�ĸ��Ʋ���2", strlen("����һ���ڴ�ĸ��Ʋ���2"));
	KdPrint(("%s\n", Buffer));

	RtlFillMemory(Buffer, 64, 0xcc);
	KdPrint(("%s\n", Buffer));

	RtlZeroMemory(Buffer, sizeof(Buffer));

	if (RtlEqualMemory(Src,Dst,sizeof(Src)))
	{
		KdPrint(("�����ڴ���ͬ\n"));

	}
	else
	{
		KdPrint(("�����ڴ治ͬ\n"));
	}
	

	
	return STATUS_SUCCESS;
}