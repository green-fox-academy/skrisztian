#include "logging.h"


int server_log(char* service, char* message, int error_code){

    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    // asctime(localtime(&ltime));


    if (strcmp(service, "broadcast_listener") == 0)
        fprintf(bl_log_fp, "%s> %s %d\n", service, message, error_code);
    else if (strcmp(service, "discovery_listener") == 0)
        fprintf(dl_log_fp, "%s> %s %d\n", service, message, error_code);
    else if (strcmp(service, "message_listener") == 0)
        fprintf(ml_log_fp, "%s> %s %d\n", service, message, error_code);
    else if (strcmp(service, "message_sender") == 0)
        fprintf(ms_log_fp, "%s> %s %d\n", service, message, error_code);
    else if (strcmp(service, "winsock") == 0)
        fprintf(w_log_fp, "%s> %s %d\n", service, message, error_code);
    else if (strcmp(service, "broadcast_sender") == 0)
        fprintf(bs_log_fp, "%s> %s %d\n", service, message, error_code);

    return 0;
}
