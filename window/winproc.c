#include <stdio.h>
#include <windows.h>
#include <WinBase.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <winnt.h>
#include <securitybaseapi.h>

BOOL SetPrivilege(LPCTSTR lpszPrivilege) {
    TOKEN_PRIVILEGES TokenPrivileges;
    TOKEN_PRIVILEGES PreviousTokenPrivileges;
    LUID luid;
    HANDLE hToken = NULL;
    DWORD dwPreviousTokenPrivilegesSize = sizeof(TOKEN_PRIVILEGES);
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return FALSE;
    }
    if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
        return FALSE;
    }
    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Luid = luid;
    TokenPrivileges.Privileges[0].Attributes = 0;
    if (ERROR_NOT_ALL_ASSIGNED == AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges,
        sizeof(TOKEN_PRIVILEGES), &PreviousTokenPrivileges, &dwPreviousTokenPrivilegesSize)) {
        return FALSE;
    }
    PreviousTokenPrivileges.PrivilegeCount = 1;
    PreviousTokenPrivileges.Privileges[0].Luid = luid;
    PreviousTokenPrivileges.Privileges[0].Attributes |= SE_PRIVILEGE_ENABLED;
    if (ERROR_NOT_ALL_ASSIGNED == AdjustTokenPrivileges(hToken, FALSE, &PreviousTokenPrivileges,
        dwPreviousTokenPrivilegesSize, NULL, NULL)) {
        return FALSE;
    }
    if (NULL != hToken) {
        ::CloseHandle(hToken);
    }
    return TRUE;
}
void ShowProcessIntegrityLevel(TCHAR* szProcessName, HANDLE hProcess, DWORD dwProcessID)
{
    HANDLE hToken = NULL;
    DWORD dwLengthNeeded = 0x00;
    DWORD dwError = ERROR_SUCCESS;
    PTOKEN_MANDATORY_LABEL pTIL = NULL;
    LPWSTR pStringSid;
    DWORD dwIntegrityLevel = 0x00;
    if (FALSE == OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        return;
    }
    if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwLengthNeeded)) {
        dwError = GetLastError();
        if (dwError == ERROR_INSUFFICIENT_BUFFER) {
            pTIL = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwLengthNeeded);
            if (pTIL != NULL) {
                if (GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwLengthNeeded,
                    &dwLengthNeeded)) {
                    dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid,
                        (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));
                    if (dwIntegrityLevel < SECURITY_MANDATORY_LOW_RID) {
                        wprintf(L"Error\n");
                    }
                    else if (dwIntegrityLevel == SECURITY_MANDATORY_LOW_RID) {
                        DWORD dwAppContainer = 0;
                        DWORD dwDummy = 0;
                        if (::GetTokenInformation(hToken, TokenIsAppContainer, &dwAppContainer,
                            sizeof(dwAppContainer), &dwDummy)) {
                            if (dwAppContainer == 1) {
                                wprintf(L"this process Appcontainer\n"); //Appcontainer프로세스

                            }
                            else {
                                wprintf(L"Low Process\n");
                            }
                        }
                    }
                    else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID &&
                        dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID) {
                        wprintf(L"Medium Process\n");
                    }
                    else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID &&
                        dwIntegrityLevel < SECURITY_MANDATORY_SYSTEM_RID) {
                        wprintf(L"High Integrity Process\n");
                    }
                    else if (dwIntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID) {
                        wprintf(L"system integrity process\n");
                    }
                    else {
                        wprintf(L"2t====process\n");
                    }
                }
                LocalFree(pTIL);
            }
        }
    }
    CloseHandle(hToken);

}
BOOL getprocess() {
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32W pe32;
    DWORD dwPriorityClass;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("hProcessSnap error\n");
        return FALSE;
    }
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Process32First error\n");
        CloseHandle(hProcessSnap);
        return FALSE;
    }
    do {
        _tprintf(TEXT("\n\n----------"));
        _tprintf(TEXT("PROCESS NAME: %s : "), pe32.szExeFile);
       
        _tprintf(TEXT("\n----------"));
        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL) {
            printf("OpenProcess Error %u\n",GetLastError());
        }
        else {
            printf("\n");
            ShowProcessIntegrityLevel(pe32.szExeFile, hProcess, pe32.th32ProcessID);
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass) {
                printf("GetPriorityClass Error\n");
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
    return TRUE;
}
int main()
{
    SetPrivilege(SE_DEBUG_NAME);
    if (getprocess() == FALSE) {
        printf("Error: getproccess..\n");
    }
    
    system("pause");
    return 0;
}
