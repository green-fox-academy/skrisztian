#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <process.h>
#include "comms.h"
#include "logging.h"
#include "userfunctions.h"






char hostname_file_name[255];
char user_data_file_name[255];



int main()
{
    // Set up parameters
    b_port = 12345;
    d_port = 54000;
    m_port = 54001;
    s_port = 54002;

    strcpy(hostname, "krisz");
    client_count = 0;

    char command;

    // Open log files
    strcpy(broadcast_listener_log_file_name, "broadcast_listener.log");
    bl_log_fp = fopen(broadcast_listener_log_file_name, "w+");

    strcpy(discovery_listener_log_file_name, "discovery_listener.log");
    dl_log_fp = fopen(discovery_listener_log_file_name, "w+");

    strcpy(message_listener_log_file_name, "message_listener.log");
    ml_log_fp = fopen(message_listener_log_file_name, "w+");

    strcpy(message_sender_log_file_name, "message_sender.log");
    ms_log_fp = fopen(message_sender_log_file_name, "w+");

    strcpy(broadcast_sender_log_file_name, "broadcast_sender.log");
    bs_log_fp = fopen(broadcast_sender_log_file_name, "w+");

    strcpy(winsock_log_file_name, "winsock.log");
    w_log_fp = fopen(winsock_log_file_name, "w+");


    int winsock_error = init_winsock();

    if (!winsock_error) {
        printf("Winsock started\n");
    } else {
        printf("Winsock failed with error: %d\n", winsock_error);
    }

    _beginthread(broadcast_listener, 0, NULL);
    printf("Broadcast listener started\n");
    _beginthread(discovery_listener, 0, NULL);
    printf("Discovery listener started\n");
    _beginthread(message_listener, 0, NULL);
    printf("Message listener started\n");




    while(1){

        command = getch();
        switch (command) {
        case 'b':
            send_broadcast(b_port);
            break;
        case 'x':
            close_winsock();
            fclose(bl_log_fp);
            fclose(dl_log_fp);
            fclose(ml_log_fp);
            fclose(ms_log_fp);
            fclose(w_log_fp);
            return 0;
            break;
        case 'l':
            list_clients();
            break;
        case 'm':
            send_chat();
            break;
        }
    }



    return 0;
}
