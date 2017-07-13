/* Includes ------------------------------------------------------------------*/
#include "led_matrix.h"
#include "characters.h"

void led_matrix_set(uint8_t row, uint8_t col, uint8_t state) {

	// Wait for the mutex
	osMutexWait(led_matrix_mutex_id, osWaitForever);

	// Change the LED in the selected row and col to the specified state
	led_matrix_state[row][col] = state;

	// Release the mutex
	osMutexRelease(led_matrix_mutex_id);

	return;
}

void led_matrix_flip_led(uint8_t row, uint8_t col) {

	// Wait for the mutex
	osMutexWait(led_matrix_mutex_id, osWaitForever);

	// Change the LED in the selected row and col to the specified state
	uint8_t state = led_matrix_state[row][col];
	led_matrix_state[row][col] = !state;

	// Release the mutex
	osMutexRelease(led_matrix_mutex_id);

	return;
}

void led_matrix_scroll_thread(void const *argument)
{
	//Get the message via mail sender
	while(1) {

		osDelay(20);

		scroll_text("Give me input! ");

		osEvent event = osMailGet(mail_q_id, osWaitForever);
		if (event.status == osEventMail) {
			mail_t* received = (mail_t*) event.value.p;       // ".p" indicates that the message is a pointer
			scroll_text(received->text);
			osMailFree(mail_q_id, received);
		}

		scroll_text("More input! ");

	}

	while (1) {
		LCD_ErrLog("scroll thread - terminating...\n");
		osThreadTerminate(NULL);
	}
}

void scroll_text(char* text)
{
	// Convert text to bitmap canvas
	uint8_t canvas[600];
	uint16_t end_of_canvas;
	memset(canvas, 0, sizeof(canvas));

	int j = 0;
	for (int i = 0; i < strlen(text); i++)
	{
		// Only if we have it in ascii_col array
		if ((text[i] >= 0x20) && (text[i] <= 0x7e)) {

			// Copy every column of the letter onto the canvas
			for (int col = 0; col < LED_MATRIX_COLS; col++) {

				// Skip empty columns if there are another
				// empty columns around them

				switch (col) {
				case 0:
					if (ascii_col[text[i] - 0x20][col] == 0) {
						// skip
					} else {
						canvas[j++] = ascii_col[text[i] - 0x20][col];
					}
					break;
				case 1:
					if ((ascii_col[text[i] - 0x20][col] == 0) &&
					    (ascii_col[text[i] - 0x20][col - 1] == 0)) {
						// skip
					} else {
						canvas[j++] = ascii_col[text[i] - 0x20][col];
					}
					break;
				case 2:
					if (((ascii_col[text[i] - 0x20][col] == 0) &&
						(ascii_col[text[i] - 0x20][col - 1] == 0) &&
						(ascii_col[text[i] - 0x20][col - 2] == 0)) ||
						((ascii_col[text[i] - 0x20][col] == 0) &&
						 (ascii_col[text[i] - 0x20][col + 1] == 0) &&
						 (ascii_col[text[i] - 0x20][col + 2] == 0))) {
						// skip
					} else {
						canvas[j++] = ascii_col[text[i] - 0x20][col];
					}
					break;
				case 3:
					if ((ascii_col[text[i] - 0x20][col] == 0) &&
					    (ascii_col[text[i] - 0x20][col + 1] == 0)) {
						// skip
					} else {
						canvas[j++] = ascii_col[text[i] - 0x20][col];
					}
					break;
				case 4:
					if (ascii_col[text[i] - 0x20][col] == 0) {
						// skip
					} else {
						canvas[j++] = ascii_col[text[i] - 0x20][col];
					}
					break;
				}
			}

			// If letter is a space print 5 empty columns
			// otherwise print a separating empty column
			if (text[i] == ' ') {
				for (int k = 0; k < LED_MATRIX_COLS; k++) {
					canvas[j++] = 0;
				}
			} else {
				canvas[j++] = 0;
			}
		}
	}

	end_of_canvas = j;

	// To update led_status_matrix we move a fixed
	// size window from left to right over the canvas
	for (int i = 0; i < end_of_canvas - LED_MATRIX_COLS; i++) {
		for (int col = 0; col < LED_MATRIX_COLS; col++) {
			for (int row = 0; row < LED_MATRIX_ROWS; row++) {
				if (canvas[i + col] & (1 << row))
					led_matrix_set(row, col, 1);
				else
					led_matrix_set(row, col, 0);
			}
		}

		// Set delay from potentiometer
		osDelay(set_delay());
	}

	return;
}

void led_matrix_update_thread(void const *argument)
{

	// Initialize the pins as outputs
	led_matrix_pin_init();

	// Create a mutex -> LED_MATRIX_MUTEX_DEF
	//led_matrix_mutex_id = osMutexCreate(osMutex(LED_MATRIX_MUTEX_DEF));

	LCD_UsrLog("led_matrix - initialized\n");

	// Infinite loop
	while (1) {

		// Iterate through every column
		for (int i = 0; i < LED_MATRIX_COLS; i++) {

			// Wait for the mutex
			osMutexWait(led_matrix_mutex_id, osWaitForever);

			// Turn on the column
			HAL_GPIO_WritePin(col_pins[i].gpio_port, col_pins[i].gpio_pin, GPIO_PIN_SET);

			// Turn on rows in that column according to what is set
			// in led_matrix_state
			for (int j = 0; j < LED_MATRIX_ROWS; j++) {
				HAL_GPIO_WritePin(row_pins[j].gpio_port, row_pins[j].gpio_pin,
						          !led_matrix_state[j][LED_MATRIX_COLS - 1 - i]);
			}

			// Release the mutex
			osMutexRelease(led_matrix_mutex_id);

			// Give processor time to other threads
			osDelay(1);

			// Turn off the column
			HAL_GPIO_WritePin(col_pins[i].gpio_port, col_pins[i].gpio_pin, GPIO_PIN_RESET);
		}
	}

	// Terminating thread
	while (1) {
		LCD_ErrLog("led_matrix - terminating...\n");
		osThreadTerminate(NULL);
	}
}

// This thread is a waterfall type animation
void led_matrix_waterfall_thread(void const *argument)
{
	led_matrix_clear();
	while (1) {
		for (uint8_t r = 0; r < LED_MATRIX_ROWS; r++) {
			for (uint8_t c = 0; c < LED_MATRIX_COLS; c++) {
				led_matrix_set(r, c, 1);
				osDelay(set_delay());
				led_matrix_set(r, c, 0);
			}
		}
	}

	while (1) {
		LCD_ErrLog("led_matrix_waterfall - terminating...\n");
		osThreadTerminate(NULL);
	}
}

// Initialize the outputs for the LED matrix
void led_matrix_pin_init(void)
{
	/* Pins used:
	 * LED MATIX Row1 - D0 - PC7
	 * LED MATIX Row2 - D1 - PC6
	 * LED MATIX Row3 - D2 - PG6
	 * LED MATIX Row4 - D3 - PB4
	 * LED MATIX Row5 - D4 - PG7
	 * LED MATIX Row6 - D5 - PI0
	 * LED MATIX Row7 - D6 - PH6
	 * LED MATIX Col1 - D7 - PI3
	 * LED MATIX Col2 - D8 - PI2
	 * LED MATIX Col3 - D9 - PA15
	 * LED MATIX Col4 - D10 - PA8
	 * LED MATIX Col5 - D11 - PB15
	 */

	GPIO_TypeDef* port[] = {GPIOC, GPIOC, GPIOG, GPIOB, GPIOG, GPIOI, GPIOH,
			                GPIOI, GPIOI, GPIOA, GPIOA, GPIOB};

	uint16_t pin[] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_6, GPIO_PIN_4,
					   GPIO_PIN_7, GPIO_PIN_0, GPIO_PIN_6, GPIO_PIN_3,
					   GPIO_PIN_2, GPIO_PIN_15, GPIO_PIN_8, GPIO_PIN_15};

	// Init GPIO clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	for (int i = 0; i < LED_MATRIX_ROWS + LED_MATRIX_COLS; i++) {

		// ConfigureGPIOs' specific pins
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = pin[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		// Init GPIO pin
		HAL_GPIO_Init(port[i], &GPIO_InitStruct);
	}

	// Fill in row and col io data arrays
	// we'll use them to update the led matrix

	for (int i = 0; i < LED_MATRIX_ROWS; i++) {
		row_pins[i].gpio_port = port[i];
		row_pins[i].gpio_pin = pin[i];
	}

	for (int i = LED_MATRIX_ROWS; i < LED_MATRIX_ROWS + LED_MATRIX_COLS; i++) {
		col_pins[i - LED_MATRIX_ROWS].gpio_port = port[i];
		col_pins[i - LED_MATRIX_ROWS].gpio_pin = pin[i];
	}

	return;
}

void led_matrix_clear(void)
{
	// Fill in all matrix values with zeros
	for (int i = 0; i < LED_MATRIX_ROWS; i++) {
		for (int j = 0; j < LED_MATRIX_COLS; j++) {
			led_matrix_set(i, j, 0);
		}
	}
	return;
}

uint16_t set_delay(void)
{
	uint16_t max_delay = 1000;
	uint16_t max_adc = 4096;
	uint16_t adc_value;

	osEvent evt = osMessageGet(adc_q_id, osWaitForever);  // wait for message
	if (evt.status == osEventMessage) {
		adc_value = (uint16_t) evt.value.v;
		float delay = ((float) adc_value / (float) max_adc) * (float) max_delay;
		return (uint16_t) delay;
	} else {
		return 0;
	}
}

void display_char_r(char letter)
{
	// Only convert characters which are in our ascii_row table
	if ((letter >= 0x20) && (letter <= 0x7e)) {

		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 5; col++) {
				if (ascii_row[letter - 0x20][row] & (1 << col))
					led_matrix_set(row, col, 1);
				else
					led_matrix_set(row, col, 0);

			}
		}
	}
	return;
}

void display_char_c(char letter)
{
	// Only convert characters which are in our ascii_row table
	if ((letter >= 0x20) && (letter <= 0x7e)) {

		for (int col = 0; col < 5; col++) {
			for (int row = 0; row < 7; row++) {
				if (ascii_col[letter - 0x20][col] & (1 << row))
					led_matrix_set(row, col, 1);
				else
					led_matrix_set(row, col, 0);

			}
		}
	}
	return;
}

void iterate_ascii_table(void)
{
	while(1) {
		for (char i = ' '; i <= '~'; i++) {
			display_char_c(i);
			osDelay(1000);
		}
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
