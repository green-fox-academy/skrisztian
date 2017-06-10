#include "temperature_logger.h"

int main()
{
    ports = comEnumerate();
    port = -1;
    const int baud_rate = 115200;
    measure_t = 0;
    message[0] = '\0';
    action = 0;
    interval = 0;
    logging = 0;
    temperature = 0;

    while(1) {

        clrscr();
        print_main_screen();
        get_command(message);

        //*** Exit from program
        if (strcmp(user_input, "e") == 0) {
            comCloseAll();
            comTerminate();
            clrscr();
            exit(0);
        //*** END Exit from program

        //*** Set file name
        } else if ((strcmp(user_input, "f") == 0) || (action == 'f')) {

            // TO-DO

        //*** END Set file name

        //*** Open or close port
        } else if ((strcmp(user_input, "p") == 0) || (action == 'p')) {

            // User wants to open port. Ask which one?
            if ((action == 0) && (port == -1)) {
                strcpy(message, "Please enter the index of the port to be open");
                action = 'p';

            // User returned with a port number to open
            } else if ((action == 'p') && (port == -1)) {

                // Check if we got a number
                if (is_number(user_input)) {

                    // Check if it is a valid number
                    int p_port = atoi(user_input);
                    if ((p_port >= 0) && (p_port <= ports - 1)) {

                        // Open port
                        if (comOpen(p_port, baud_rate)) {
                            port = p_port;
                            strcpy(message, "Port ");
                            strcat(message, comGetPortName(port));
                            strcat(message, " is now open.");
                            action = 0;

                        // If cannot open port give error
                        } else {
                            char tmp[10];
                            strcpy(message, "Cannot open port ");
                            strcat(message, itoa(p_port, tmp, 10));
                            strcat(message, ".");
                            action = 0;
                        }

                    // The number is out of range
                    } else {
                        strcpy(message, user_input);
                        strcat(message, " is out of range for a port index.");
                        action = 0;
                    }

                // It was not a number
                } else {
                    strcpy(message, user_input);
                    strcat(message, " is not a number.");
                    action = 0;
                }

            // A port is open, stop activities and close it
            } else if ((action == 0) && (port != -1)) {
                stop_measurement();
                stop_logging();
                comClose(port);
                temperature = 0;
                measure_t = 0;
                strcpy(message,"Port ");
                strcat(message, comGetPortName(port));
                strcat(message, " is closed now. All activities stopped.");
                port = -1;
                action = 0;

            // We should not be here
            } else {
                general_error();
            }
        //*** END Open or close port

        //*** Start/stop logging
        } else if ((strcmp(user_input, "l") == 0) || (action == 'l')) {

            // TO-DO

        //*** END Start logging

        //*** Start/stop continuous measurement
        } else if ((strcmp(user_input, "c") == 0) || (action == 'c')) {

            // Start measurement
            if ((measure_t != CONT) && (action == 0)) {

                // No port connected
                if (port == -1) {
                    strcpy(message, "You need to open a port first.");
                    action = 0;

                // Port connected, ask for polling rate
                } else {
                    strcpy(message, "Select polling period. Enter 1 for 30 sec or 2 for 300 sec.");
                    action = 'c';
                }

            // Set polling rate and start measurement
            } else if ((measure_t != CONT) && (action == 'c')) {
                if ((strcmp(user_input, "1") == 0) || (strcmp(user_input, "2") == 0)) {
                    start_continuous_measurement(user_input[0]);
                    action = 0;

                // Wrong choice received
                } else {
                    strcpy(message, user_input);
                    strcat(message, " is not a valid answer.");
                    action = 0;
                }

            // Stop measurement
            } else {
                stop_c_measurement();
            }

        //*** END Start/stop continuous measurement

        //*** Start a single measurement
        } else if ((strcmp(user_input, "s") == 0) || (action == 's')) {

            // Check if port connected
            if (port == -1) {
                strcpy(message, "You need to open a port first.");
                action = 0;
            } else {
                single_measurement();
                action = 0;
            }

        //*** END Start a single measurement

        //*** Calculate average
        } else if ((strcmp(user_input, "a") == 0) || (action == 'a')) {

            // TO-DO

        //END *** Calculate average

        // *** Not a valid command
        } else {
            strcpy(message, user_input);
            strcat(message, " is not a valid command.");
        }

        // END *** Not a valid command
    }
    return 0;
}
