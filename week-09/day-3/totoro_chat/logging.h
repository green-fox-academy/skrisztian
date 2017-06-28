#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

// String for logging
char log_msg[255];

int server_log(char* service, char* message, int error_code);

#endif // LOGGING_H_INCLUDED
