.386
.model flat,stdcall
option casemap:none

include ntstatus.inc
include ntddk.inc
include ntoskrnl.inc

.data
szFmt1	db	"��������",0ah,0
szFmt2	db	"����ж��",0ah,0

.code
Unload	proc DriverObject:PDRIVER_OBJECT
	invoke DbgPrint,offset szFmt2
	ret
Unload  endp

DriverEntry proc DriverObject:PDRIVER_OBJECT,RegistryPath:PUNICODE_STRING
	
	invoke DbgPrint,offset szFmt1


	mov eax,DriverObject
	assume eax:PDRIVER_OBJECT
	mov [eax].DriverUnload,offset Unload
	assume eax:nothing


	mov eax,STATUS_SUCCESS
	ret
DriverEntry	endp
end



