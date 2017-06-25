 /**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    22-April-2016 
  * @brief   STM32F7xx HAL API Template project 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32746g_discovery_lcd.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct {
	uint32_t ovf;
	uint32_t prev;
	uint32_t last;
} input_capture_data_t;

/* Private define ------------------------------------------------------------*/
//#define USE_P_CTRLER


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ADC_HandleTypeDef adc3_handle;
static ADC_ChannelConfTypeDef adc3_ch_conf;
static TIM_HandleTypeDef tim3_handle;
static TIM_OC_InitTypeDef tim3_oc_conf;
static TIM_HandleTypeDef tim2_handle;
static TIM_IC_InitTypeDef tim2_ic_conf;
static uint16_t adc_timeout;
static uint16_t adc_value;



/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void set_pwm_value_a(uint16_t pwm_value);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This project template calls firstly two functions in order to configure MPU feature
     and to enable the CPU Cache, respectively MPU_Config() and CPU_CACHE_Enable().
     These functions are provided as template implementation that User may integrate
     in his application, to enhance the performance in case of use of AXI interface
     with several masters. */

  /* Configure the MPU attributes as Write Through */
  MPU_Config();

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


  /* Add your application code here */
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(0);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LED_Init(LED_GREEN);

  //*** ADC - A0 PA0 ADC3_IN0 ***
  adc3_handle.Instance = ADC3;
  adc3_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  adc3_handle.Init.ContinuousConvMode = DISABLE;
  adc3_handle.Init.Resolution = ADC_RESOLUTION_12B;

  adc3_ch_conf.Channel = ADC_CHANNEL_0;
  adc3_ch_conf.SamplingTime = ADC_SAMPLETIME_480CYCLES;

  adc_timeout = 100;	// ms

  // Init ADC
  HAL_ADC_Init(&adc3_handle);

  // Configure ADC channels;
  HAL_ADC_ConfigChannel(&adc3_handle, &adc3_ch_conf);

  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)"Startup");

  // *** PWM - D3 PB4 TIM3_CH1 ***
  tim3_handle.Instance = TIM3;
  tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  tim3_handle.Init.Period = 4095;
  tim3_handle.Init.Prescaler = 0;

  tim3_oc_conf.OCMode = TIM_OCMODE_PWM1;
  tim3_oc_conf.OCFastMode = TIM_OCFAST_DISABLE;
  tim3_oc_conf.Pulse = 2047;
  tim3_oc_conf.OCPolarity = TIM_OCPOLARITY_HIGH;

  	// Init PWM
	HAL_TIM_PWM_Init(&tim3_handle);

	// Configure PWM channel
	HAL_TIM_PWM_ConfigChannel(&tim3_handle, &tim3_oc_conf, TIM_CHANNEL_1);

	// Start PWM
	HAL_TIM_PWM_Start(&tim3_handle, TIM_CHANNEL_1);


	// *** Input capture - D9 PA15 TIM2_CH1 ***
	tim2_handle.Instance = TIM2;
	tim2_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2_handle.Init.Period = 0xffff;
	tim2_handle.Init.Prescaler = 0;
	tim2_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	tim2_handle.State = HAL_TIM_STATE_RESET;

	tim2_ic_conf.ICFilter = 0; // number between 0x0 and 0xf
	tim2_ic_conf.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2_ic_conf.ICPrescaler = TIM_ICPSC_DIV1;	// input captured every 1 rising edge
	tim2_ic_conf.ICSelection = TIM_ICSELECTION_DIRECTTI;

	// Init IC timer
	HAL_TIM_IC_Init(&tim2_handle);

	// Config IC timer
	HAL_TIM_IC_ConfigChannel(&tim2_handle, &tim2_ic_conf, TIM_CHANNEL_1);

	// Start normal and IC timer
	HAL_TIM_Base_Start_IT(&tim2_handle);
	HAL_TIM_IC_Start_IT(&tim2_handle, TIM_CHANNEL_1);




  int8_t cntr = 0;
  /* Infinite loop */
  while (1)
  {
	  char buff[100];

	  // Start ADC conversion
	  HAL_ADC_Start(&adc3_handle);

	  // Wait for ADC poll to finish
	  HAL_ADC_PollForConversion(&adc3_handle, adc_timeout);

	  // Read ADC conversion value
	  adc_value = HAL_ADC_GetValue(&adc3_handle);
	  set_pwm_value_a(adc_value);

	  // Print out value
	  sprintf(buff, "%d", adc_value);

	  BSP_LCD_ClearStringLine(0);
	  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)buff);
	  cntr++;
	  BSP_LED_Toggle(LED_GREEN);
	  HAL_Delay(100);
  }
} // END of main

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim2_handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// Normal timer interrupt
	// flash led
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// Input capture timer interrupt
}



void set_pwm_value_a(uint16_t pwm_value)
{
	if (pwm_value > 4095)
		pwm_value = 4095;

	tim3_oc_conf.Pulse = pwm_value;
	HAL_TIM_PWM_ConfigChannel(&tim3_handle, &tim3_oc_conf, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim3_handle, TIM_CHANNEL_1);

	return;
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
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
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
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
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

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
