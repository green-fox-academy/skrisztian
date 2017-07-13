#include <winsock2.h>
#include "winsock_service.h"

int start_winsock(void)
{
    char service[20] = "winsock";

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        int error = WSAGetLastError();
        server_log(service, "WSAStartup failed with error: ", error);
        return error;
    } else {
        server_log(service, "WSAStartup OK.", 0);
    }

    winsock_up = 1;

    return 0;
}

int close_winsock(void)
{
    char service[20] = "winsock";
    winsock_up = 0;

    // Close winsock
    if (WSACleanup() != 0) {
       int error = WSAGetLastError();
        server_log(service, "WSACleanup failed with error: ", error);
        return error;
    } else {
        server_log(service, "WSACleanup OK.", 0);
    }

    return 0;
}


