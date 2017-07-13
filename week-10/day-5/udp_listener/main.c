#include <stdio.h>
#include <stdlib.h>
#include "udp_server.h"
#include "winsock_service.h"

int main()
{
    char cmd;
    char log_file[] = "udpserver.log";
    // Start logging
    // Open log files
    log_fp = fopen(log_file, "w+");


    winsock_up = 0;
    start_winsock();
    _beginthread(udp_server_thread, 0, NULL);

    printf("Ready.\n");
    printf("Enter 'x' for exit.\n");


    while(1) {

        cmd = getch();

        switch (cmd) {
        case 'x':
            close_winsock();
            fclose(log_fp);
            return 0;
            break;
        default:
            printf("Command not recognized.\n");
            break;
        }
    }

    return 0;
}
