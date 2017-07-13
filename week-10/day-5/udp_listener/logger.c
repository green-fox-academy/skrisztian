#include "logger.h"
#include <time.h>

int server_log(char* service, char* message, int error_code){

    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    // asctime(localtime(&ltime));

    fprintf(log_fp, "%s> %s %d\n", service, message, error_code);

    return 0;
}

