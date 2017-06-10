#ifndef TEMPERATURE_LOGGER_H_INCLUDED
#define TEMPERATURE_LOGGER_H_INCLUDED

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "rs232.h"

#define SINGLE 1
#define CONT 2

// Global variables

int ports;
int port;
int measure_t;
char user_input[255];
char read_file[255];
char write_file[255];
char message[255];
char action;
int interval;
int logging;
int temperature;

// Functions

void list_ports(int ports);
void set_port(int port);
void open_port(int port);
void set_write_file(char *file_name);
void set_read_file(char *file_name);
void toggle_logging(void);
void close_port(int port);
void print_average(void);
void clrscr(void);
void print_usage(void);
void print_cont_measurement();
void print_main_screen(void);
int length(const char *array);
void get_command(char *message);
void stop_logging(void);
void stop_measurement(void);
int is_number(char *string);
void general_error(void);
void print_interval(void);
void print_logging(void);
void single_measurement(void);
void start_continuous_measurement(char);
void stop_c_measurement(void);

#endif // TEMPERATURE_LOGGER_H_INCLUDED

