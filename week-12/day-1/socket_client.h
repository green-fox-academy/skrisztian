#ifndef __SOCKET_CLIENT_H
#define __SOCKET_CLIENT_H

#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include <string.h>

#define REMOTE_IP		"10.27.6.120"
#define REMOTE_PORT		54545

#define MAX_TRY			60
#define CLIENT_BUFF_LEN	256

void socket_client_thread(void const *argument);

#endif /* __SOCKET_CLIENT_H */