#ifndef COMMS_H_INCLUDED
#define COMMS_H_INCLUDED

#include <winsock2.h>
#include "logging.h"
#include <stdio.h>
#include <string.h>




// Host data
int s_port; // sending port
int m_port; // message receiving port
int d_port; // discovery receiving port
int b_port; // broadcast receiving port
char hostname[255];

int run_msg_server;


void broadcast_server(void);

#endif // COMMS_H_INCLUDED

