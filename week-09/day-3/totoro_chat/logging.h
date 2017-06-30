#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <stdio.h>
#include <time.h>



// String for logging
FILE *bl_log_fp;
FILE *dl_log_fp;
FILE *ml_log_fp;
FILE *ms_log_fp;
FILE *bs_log_fp;
FILE *w_log_fp;


// File names
char broadcast_listener_log_file_name[255];
char discovery_listener_log_file_name[255];
char message_listener_log_file_name[255];
char message_sender_log_file_name[255];
char broadcast_sender_log_file_name[255];
char winsock_log_file_name[255];

int server_log(char* service, char* message, int error_code);

#endif // LOGGING_H_INCLUDED
