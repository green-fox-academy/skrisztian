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
    int d_port;
    int m_port;
} client_t;

// Array to store all clients's data structures
client_t clients[50];



// File names
char server_log_file_name[255];
char hostname_file_name[255];
char user_data_file_name[255];



int main()
{
    b_port = 12345;


    printf("Hello world!\n");

    _beginthread(broadcast_server, 0, NULL);

    while(1);

    return 0;
}
