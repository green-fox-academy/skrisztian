#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdio.h>

FILE* log_fp;

int server_log(char* service, char* message, int error_code);

#endif // __LOGGER_H
