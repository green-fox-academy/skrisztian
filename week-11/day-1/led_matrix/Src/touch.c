#include "touch.h"

void screen_thread(void const *argument)
{

	init_screen();

	while(1) {

		for (int x = 0; x < LED_MATRIX_ROWS; x++) {
			osMutexWait(led_matrix_mutex_id, osWaitForever);
			for (int y = 0; y < LED_MATRIX_COLS; y++) {

				// Draw red circle for leds on, white for off
				if (led_matrix_state[x][y] == GPIO_PIN_SET) {
					BSP_LCD_SetTextColor(LCD_COLOR_RED);
				} else {
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				}

				BSP_LCD_FillCircle(BUTTON_WIDTH * (x) + BUTTON_WIDTH / 2,
						(BUTTON_WIDTH * LED_MATRIX_COLS) - BUTTON_WIDTH * (y + 1) + BUTTON_WIDTH / 2,
						BUTTON_WIDTH / 2 - 5 );
			}
			osMutexRelease(led_matrix_mutex_id);
		}

		osDelay(5);
	}

	// Terminating thread
	while (1) {
		LCD_ErrLog("screen - terminating...\n");
		osThreadTerminate(NULL);
	}
}


void touch_thread(void const *argument)
{
	coord_t touch_current;
	coord_t led_previous;
	uint8_t finger_down = 0;
	uint8_t changed = 0;
	init_screen();

	led_previous.x = 100;
	led_previous.y = 100;
	led.x = 101;
	led.y = 101;

	while(1) {

		// Read touch values
		BSP_TS_GetState(&TS_State);
		if (TS_State.touchDetected) {

			led_previous.x = led.x;
			led_previous.y = led.y;

			touch_current.x = TS_State.touchX[0];
			touch_current.y = TS_State.touchY[0];

			// We only care about touches over the led dots
			if ((touch_current.x <= BUTTON_WIDTH * 7) &&
				(touch_current.y <= BUTTON_WIDTH * 5)) {

					// Set led coordinates
					led.x = touch_current.x / BUTTON_WIDTH;
					led.y = 4 - touch_current.y / BUTTON_WIDTH;
			}

			// Reset changed flag if we move over another led dot
			if (led.x != led_previous.x || led.y != led_previous.y)
				changed = 0;

			// Sticky change to avoid dot flickering
			if (finger_down && !changed) {
				led_matrix_flip_led(led.x, led.y);
				changed = 1;
			}

			finger_down = 1;

		} else {
			finger_down = 0;
			changed = 0;
		}

		osDelay(5);
	}

	// Terminating thread
	while (1) {
		LCD_ErrLog("touch thread - terminating...\n");
		osThreadTerminate(NULL);
	}
}

void init_screen(void)
{
	max_x = BSP_LCD_GetXSize();
	max_y = BSP_LCD_GetYSize();

	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0, 0, 380, 272);

	return;
}



