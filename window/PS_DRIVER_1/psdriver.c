#include <wdm.h>
#define NAME_OFFSET 0x5a8
#define PID_OFFSET 0x440
#define LINK_OFFSET 0x448
#define VIRTUALSIZE_OFFSET 0x498

void OnUnload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("Unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath) {
	
	//unused main parameters
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegistryPath);

	//getting EPROCESS pointer
	PEPROCESS pEPROC = (PEPROCESS)PsGetCurrentProcess();
	PLIST_ENTRY pHead, pCurr;
	unsigned char* PROC = NULL;
	
	//getting first link
	pHead = pCurr = (PLIST_ENTRY)((unsigned char*)pEPROC + LINK_OFFSET);
	
	
	do {
		PROC = (unsigned char*)((unsigned char*)pCurr - LINK_OFFSET);
		DbgPrint("Process Name:%s\n", (unsigned char*)((unsigned char*)PROC + NAME_OFFSET));
		DbgPrint("PID:%d, ", * (((unsigned char*)PROC + PID_OFFSET)));
		DbgPrint("virtual memory size: %d(kb) \n\n", ((int)(((unsigned char*)PROC + VIRTUALSIZE_OFFSET)))>>20);
		pCurr = pCurr->Flink;
	} while (pCurr->Flink != pHead);
	pDriverObject->DriverUnload = OnUnload;
	return STATUS_UNSUCCESSFUL;
}