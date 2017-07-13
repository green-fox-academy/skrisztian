#ifndef __ADC_H_INCLUDED
#define __ADC_H_INCLUDED


#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
#include "lcd_log.h"
#include "stm32746g_discovery_ts.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

static ADC_HandleTypeDef adc_h;
static ADC_ChannelConfTypeDef adc_ch_conf;
static uint16_t adc_timeout;

osMessageQId adc_q_id;           // Declare an ID for the message queue

void adc_thread(void const *argument);
void adc_init(void);
uint16_t read_adc_value(void);


#endif // __ADC_H_INCLUDED
