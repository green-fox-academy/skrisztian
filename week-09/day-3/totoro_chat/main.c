#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <process.h>
#include "comms.h"
#include "logging.h"
#include "userfunctions.h"

// Structure to store individual client's data
typedef struct {
    char name[255];
    char ip[20];
    int m_port;
} client_t;

// Array to store all clients's data structures
client_t clients[50];

// Counter for array
int client_count;



// File names
char server_log_file_name[255];
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


            return 0;
            break;
        }
    }



    return 0;
}
