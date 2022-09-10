#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char** argv)
{
    WSADATA wsaData;
    struct hostent* remoteHost;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return 1;
    }
    remoteHost = gethostbyname("t1566.purpleteam.ru");
    return 0;
}
