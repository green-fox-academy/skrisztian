#ifndef COMMS_H_INCLUDED
#define COMMS_H_INCLUDED

#include <winsock2.h>
#include "logging.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Structure to store individual client's data
typedef struct {
    char name[255];
    int32_t ip;
    int m_port;
} client_t;

// Array to store all clients's data structures
client_t clients[50];

// Counter for array
int client_count;


// Host data
int s_port; // sending port
int m_port; // message receiving port
int d_port; // discovery receiving port
int b_port; // broadcast receiving port
char hostname[255];

int init_winsock(void);
int close_winsock(void);

void broadcast_listener(void);
void message_listener(void);
void discovery_listener(void);

int send_msg(int32_t remmote_ip, int remote_port, char* message);
int send_broadcast(int remote_port);
int save_user(char *name, int32_t remote_ip, int remote_port);

#endif // COMMS_H_INCLUDED

