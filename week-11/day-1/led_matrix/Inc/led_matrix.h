#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "adc.h"
#include "socket_server.h"

#define LED_MATRIX_COLS		5
#define LED_MATRIX_ROWS		7

typedef struct {
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_pin;
} io_pin_t;

// Each LED state is stored in this 2D array
GPIO_PinState led_matrix_state[LED_MATRIX_ROWS][LED_MATRIX_COLS];

// Mutex global variable
osMutexId led_matrix_mutex_id;

// Global IO pin settings
io_pin_t row_pins[LED_MATRIX_ROWS];
io_pin_t col_pins[LED_MATRIX_COLS];

void led_matrix_set(uint8_t row, uint8_t col, uint8_t state);
void led_matrix_flip_led(uint8_t row, uint8_t col);
void led_matrix_update_thread(void const *argument);
void led_matrix_waterfall_thread(void const *argument);
void led_matrix_scroll_thread(void const *argument);
void scroll_text(char* text);
void led_matrix_pin_init(void);
void led_matrix_clear(void);
void display_char_r(char letter);
void display_char_c(char letter);
uint16_t set_delay(void);
void iterate_ascii_table(void);

#endif /* __LED_MATRIX_H */
