/**
 ******************************************************************************
 * @file    USB_Device/HID_Standalone/Src/main.c
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    30-December-2016
 * @brief   USB device HID demo main file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct {
	int32_t x;
	int32_t y;
} coordinate_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;
uint8_t HID_Buffer[4];
TS_StateTypeDef  TS_State;
coordinate_t touch_current;
coordinate_t touch_previous;
char string_buffer[25] ="";

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
uint8_t check_movement(coordinate_t previous, coordinate_t actual);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	 - Configure the Flash ART accelerator on ITCM interface
	 - Configure the Systick to generate an interrupt each 1 msec
	 - Set NVIC Group Priority to 4
	 - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the System clock to have a frequency of 216 MHz */
	SystemClock_Config();

	/* Configure LED1 */
	BSP_LED_Init(LED1);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	/* Init Device Library */
	USBD_Init(&USBD_Device, &HID_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

	/* Start Device Process */
	USBD_Start(&USBD_Device);
	
	// Init touch screen
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	// Init LCD
	BSP_LCD_Init();
	BSP_LCD_DisplayOn();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetLayerVisible(0, ENABLE);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);

	// This byte contains the button states
	// 0b00000001 - the left mouse button is pressed
	// 0b00000010 - the right mouse button is pressed
	// 0b00000100 - the middle mouse button is pressed
	HID_Buffer[0] = 0;
	// This byte contains the x realative movement
	HID_Buffer[1] = 0; // 10
	// This byte contains the y realative movement
	HID_Buffer[2] = 0;

	uint8_t finger_down = 0;
	uint8_t move_since_last_finger_up = 0;
	uint32_t click_timer = 0;
	uint32_t double_click_timer = 0;
	uint8_t enable_double_click_timer = 0;
	uint8_t sticky_click = 0;

	// Calculate wait time

	while (1) {

		BSP_LCD_Clear(LCD_COLOR_WHITE);

		// Read touch values
		BSP_TS_GetState(&TS_State);

		// There is an actual touch
		if(TS_State.touchDetected) {

			// If we are after a click and still in a double-click
			// time-frame, hold the mouse button down
			if (enable_double_click_timer)
				sticky_click = 1;

			// This is a continuous touch
			if (finger_down) {
				// Save previous X and Y position of touch
				touch_previous.x = touch_current.x;
				touch_previous.y = touch_current.y;

			// This is the moment we first touch the touchpad
			} else {
				// Reset timer
				click_timer = 0;
			}

			// Get current X and Y positions
			touch_current.x = TS_State.touchX[0];
			touch_current.y = TS_State.touchY[0];

			// Draw a circle at touch
			BSP_LCD_FillCircle(touch_current.x, touch_current.y, 25);

			// Dragging finger
			if (finger_down && check_movement(touch_previous, touch_current)) {

				// Calculate movement
				HID_Buffer[1] = (uint8_t) 3 * (touch_current.x - touch_previous.x);
				HID_Buffer[2] = (uint8_t) 3 * (touch_current.y - touch_previous.y);

				move_since_last_finger_up = 1;

				// If we are after a click and still in a double-click
				// time-frame, hold the mouse button down
				if (sticky_click)
					HID_Buffer[0] = 1;

			} else {
				HID_Buffer[1] = 0;
				HID_Buffer[2] = 0;
			}

			finger_down = 1;
			click_timer++;

		// No touch, finger is lifted up
		} else {

			// Click down
			if (finger_down && !move_since_last_finger_up && (click_timer < 50)) {

				// Send left button down
				HID_Buffer[0] = 1;

				// Start double click timer
				enable_double_click_timer = 1;
				double_click_timer = 0;


			// No click or released click
			} else {

				// Send left button up
				HID_Buffer[0] = 0;

				// Turn off sticky click
				sticky_click = 0;
			}

			// Send no movement
			HID_Buffer[1] = 0;
			HID_Buffer[2] = 0;

			// Clear touch flags
			finger_down = 0;
			move_since_last_finger_up = 0;
		}

		if (BSP_PB_GetState(BUTTON_KEY)) {
			HID_Buffer[0] = 1;
		}

		if (enable_double_click_timer) {

			// Increase timer counter
			double_click_timer++;

			// Disable timer after 500 ms
			if (double_click_timer >= 50) {
				enable_double_click_timer = 0;
			}
		}

		// Send USB report
		USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);

		// Wait
		HAL_Delay(10);
	}
}

uint8_t check_movement(coordinate_t previous, coordinate_t actual)
{
	if ((previous.x - actual.x == 0) && (previous.y - actual.y == 0))
		return 0;	// No movement
	else
		return 1;	// Movement
}


/**
 * @brief This function provides accurate delay (in milliseconds) based
 *        on SysTick counter flag.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param Delay: specifies the delay time length, in milliseconds.
 * @retval None
 */

void HAL_Delay(__IO uint32_t Delay) {
	while (Delay) {
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			Delay--;
		}
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 216000000
 *            HCLK(Hz)                       = 216000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 432
 *            PLL_P                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 7
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* Activate the OverDrive to reach the 216 Mhz Frequency */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
	/* User may add here some code to deal with this error */
	while (1) {
	}
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void) {
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
