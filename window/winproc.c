#include <windows.h>
#include <WinBase.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>


BOOL GetProcessList() {
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf("error");
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		printf("error! process32First");
		CloseHandle(hProcessSnap);
		return(FALSE);
	}

	do {
		_tprintf(TEXT("\n\n--------"));
		_tprintf(TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile);
		_tprintf(TEXT("\n----------"));
		dwPriorityClass = 0;
	
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

		if (hProcess == NULL) {
			printf("error:openprocess: %u\n", GetLastError());
		}
		else {
			dwPriorityClass = GetPriorityClass(hProcess);
			if (!dwPriorityClass) {
				printf("error:getpriorityclass");
			}
			_tprintf(TEXT("\n Process ID= 0x%08X"), pe32.th32ProcessID);
			_tprintf(TEXT("\n Thread count = %d"), pe32.cntThreads);
			_tprintf(TEXT("\n Parent process ID = 0x%08X"), pe32.th32ParentProcessID);
			_tprintf(TEXT("\n Priority base = %d"), pe32.pcPriClassBase);
			if (dwPriorityClass)
				_tprintf(TEXT("\n Priority class = %d"), dwPriorityClass);
		}

	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return(TRUE);
}

int main()
{
	GetProcessList();
	return 0;
}
