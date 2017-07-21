#ifndef __TOUCH_H_INCLUDED
#define __TOUCH_H_INCLUDED

#include <stdio.h>
#include "stm32746g_discovery_ts.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include <string.h>
#include "led_matrix.h"

#define BUTTON_WIDTH 54

uint16_t max_x;
uint16_t max_y;

TS_StateTypeDef TS_State;

typedef struct {
	uint16_t x;
	uint16_t y;
} coord_t;

coord_t touch;
coord_t led;

void screen_thread(void const *argument);
void touch_thread(void const *argument);
void init_screen(void);

#endif // __TOUCH_H_INCLUDED
