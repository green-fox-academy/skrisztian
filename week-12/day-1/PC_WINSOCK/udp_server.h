#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <winsock2.h>

#define UDP_SERVER_PORT 	54002
#define UDP_RECV_BUFFER_LEN	1024

void udp_server_thread(void);


#endif // __UDP_SERVER_H

