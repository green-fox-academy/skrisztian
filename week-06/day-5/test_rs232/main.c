#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rs232.h"


int main()
{
    int port = 1;
    int baud_rate = 115200;
    char user_input[10];
    char send_buffer[10] = "m";

    // Enumerate ports
    int ports = comEnumerate();
    printf("found %d ports\n", ports);

    // Open port
    int open_status = comOpen(port, baud_rate);
    printf("port open: %d\n", open_status);

    // ports = comEnumerate();

    // Send data
    int bytes_sent = comWrite(port, send_buffer, 10);
    printf("%d bytes sent\n", bytes_sent);

    comClose(port);
    comTerminate();

    return 0;
}
