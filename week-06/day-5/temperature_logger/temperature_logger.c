#include "temperature_logger.h"

// Screen contents

const char top_screen[][80] = {
    "\t\t\tTemperature logger",
    ""};

const char left_screen[][80] = {
    " Commands                                ",
    " ======================================= ",
    " p     Open/close port                   ",
    " s     Get single measurement            ",
    " c     Start/stop continuous measurement ",
    " f     Set filenames                     ",
    " l     Start / stop logging              ",
    " a     Calculate average in given period ",
    " e     Exit from the program             ",};

const char right_screen[][80] = {
    " Measurement status",
    " ======================================= ",
    " Interval: ",
    " Logging:  ",
    "",
    "",
    " Port status",
    " ======================================= ",
    " Index       Name    In use"};

const char padding[] = "                                            ";

const char hl[] = " ================================================================================== ";

void print_main_screen(void)
{
   // Print title
   for (int i = 0; i < 2; i++) {
        printf("%s\n", top_screen[i]);
   }

    // Print commands, measurement status, port status header
   for (int i = 0; i < 9; i++) {
        printf("%s  %s", left_screen[i], right_screen[i]);
        if (i == 2)
            print_interval();
        else if (i == 3)
            print_logging();
        printf("\n");
   }

    // Print port status
    for (int i = 0; i< ports; i++) {
        printf("%s", padding);
        printf("%d\t\t%s", i, comGetPortName(i));
        if (i == port)
            printf("\t*");
        printf("\n");
    }

    // Print files
    printf(" Files\n");
    printf("%s\n", hl);
    printf(" Read from:  %s\n", read_file);
    printf(" Write into: %s\n", write_file);
    printf("\n");

    // Print measurement
    printf(" Measurement");
    if (measure_t == 1)
        printf(" - single");
    else if (measure_t == 2)
        printf(" - continuous");
    printf("\n");
    printf("%s\n", hl);
    printf(" Temperature [C]:\t%d\n", temperature);
    printf(" Humidity [\%]:\t\n");
    printf(" Pressure [Bar]:\t\n");

    // Print empty line
    printf("\n");

    return;
}

void print_interval(void)
{
    if (interval)
        printf("%d sec", interval);
    else
        printf("not set");

    return;
}

void print_logging(void)
{
    if (logging)
        printf("active");
    else
        printf("inactive");

    return;
}

void get_command(char *message)
{
    printf(" Enter command:\n");
    if (strlen(message))
        printf(" >> %s\n", message);
    printf(" >> ");
    message[0] = '\0';
    gets(user_input);

    return;
}

void general_error(void)
{
    strcpy(message, "Something went terribly wrong. Please try again.");
    action = 0;
    return;
}

void clrscr(void)
{
    system("@cls||clear");
    return;
}

void stop_logging(void)
{

}

void stop_measurement(void)
{

}

int is_number(char *string)
{
    int not_digit = 0;
    for (int i = 0; i < strlen(string); i++) {
        not_digit += (!isdigit(string[i]));
    }
    return (!not_digit);
}

void single_measurement(void)
{
    char send_buffer[1];
    char read_buffer[10];
    char temp_string[10];
    send_buffer[0] = '1';
    int i = 0;

    // Send command to MCU for single measurement
    if (comWrite(port, send_buffer, 1)) {
        strcpy(message, "Asked for a single measure.");
    } else {
        strcpy(message, "Could not connect to device. Please try again.");
        return;
    }

    // Wait for answer
    while (1) {
        if(comRead(port, read_buffer, 1)) {
            if (read_buffer[0] == '\n') {
                temp_string[i] = '\0';
                temperature = atoi(temp_string);
                measure_t = 1;
                break;
            } else {
                temp_string[i] = read_buffer[0];
            }
            i++;
        }
    }

    return;
}

void start_continuous_measurement(char m_type)
{
    char send_buffer[1];
    send_buffer[0] = m_type;

    // Send command to MCU for continuous measurement
    if(comWrite(port, send_buffer, 1)) {
        strcpy(message, "Started continuous measurement");
        measure_t = 2;
        switch (m_type) {
        case '1':
            interval = 30;
            break;
        case '2':
            interval = 300;
            break;
        }
    } else {
        strcpy(message, "Could not connect to device. Please try again.");
    }

    return;
}

void stop_c_measurement(void)
{
    char send_buffer[1];
    send_buffer[0] = '0';
    interval = 0;
    measure_t = 0;

    // Send command to MCU for single measurement
    if (comWrite(port, send_buffer, 1))
        strcpy(message, "Continuous measurement stopped.");
    else
        // If this is the case we won't get measurement either
        // but in any case let the user now about it
        strcpy(message, "Measuring device is not responding.");

    return;
}

void print_cont_measurement()
{
    int port_index = 1;
    char buffer[10] = "";
    int baud_rate = 115200;

    if (comOpen(port_index, baud_rate)) {
        printf("port %d open\n", port_index);
    };

    if (comWrite(port_index, "m", 1))
        printf("command sent\n");


    char temp_string[10] = "";
    int i = 0;

    while (1) {
        if(comRead(port_index, buffer, 1)) {
            printf("buffer: %s\n", buffer);
            if (buffer[0] == '\n') {
                temp_string[i] = '\0';
                i = -1;
            } else {
                temp_string[i] = buffer[0];
            }
            i++;
            printf("temp_string: %s\n", temp_string);
        }
    }
}
