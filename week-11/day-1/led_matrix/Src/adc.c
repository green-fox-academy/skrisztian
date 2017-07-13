#include "adc.h"

void adc_thread(void const *argument)
{
	adc_init();

	while(1){
		uint32_t adc_value = (uint32_t) read_adc_value();

		// Send data via msg queue to another thread
		osMessagePut(adc_q_id, adc_value, osWaitForever);
		osDelay(50);

	}

	// Terminate thread
	while (1) {
		LCD_ErrLog("ADC - terminating...\n");
		osThreadTerminate(NULL);
	}
}

void adc_init(void)
{
	// A3 Port F8 ADC3_IN6
	adc_h.Instance = ADC3;
	adc_h.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_h.Init.ContinuousConvMode = DISABLE;
	adc_h.Init.Resolution = ADC_RESOLUTION_12B;

	adc_ch_conf.Channel = ADC_CHANNEL_6;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	adc_timeout = 100;	// ms

	// Init ADC
	HAL_ADC_Init(&adc_h);

	// Configure ADC channels;
	HAL_ADC_ConfigChannel(&adc_h, &adc_ch_conf);

	return;
}

uint16_t read_adc_value(void)
{
	// Start ADC conversion
	HAL_ADC_Start(&adc_h);

	// Wait for ADC poll to finish
	HAL_ADC_PollForConversion(&adc_h, adc_timeout);

	// Read ADC conversion value
	uint16_t adc_value = HAL_ADC_GetValue(&adc_h);

	HAL_ADC_Stop(&adc_h);

	// Clear LCD screen at text
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(385, 10, (uint8_t*) "        ", LEFT_MODE);

	// Print to LCD screen
	char buff[9];
	sprintf(buff, "adc:%04d", adc_value);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(385, 10, (uint8_t*) buff, LEFT_MODE);

	return adc_value;
}






