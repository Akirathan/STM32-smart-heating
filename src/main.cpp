#include "main.hpp"

#define MY_DEBUG

// This section is required when using older gcc.
/*
 * The default pulls in about 12K of garbage
 */
/*extern "C" void __cxa_pure_virtual() {
  for(;;);
}*/

/*
 * Implement C++ new/delete operators using the heap
 */

/*void *operator new(size_t size) {
  return malloc(size);
}

void *operator new[](size_t size) {
  return malloc(size);
}

void operator delete(void *p) {
  free(p);
}

void operator delete[](void *p) {
  free(p);
}*/


/* Private function prototypes */
void SystemClock_Config(void);

/**
 * Handler for second interrupts from
 * RTC peripheral.
 */
void second_it() {

}

void error_handler() {
	BSP_LED_Init(LED_RED);
	BSP_LED_On(LED_RED);
}

void interval_frame_test() {
	if (BSP_LCD_Init() != IO_OK) {
		error_handler();
	}

	if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK) {
		error_handler();
	}

	interval_frame intv_fr;
	intv_fr.draw_header();
	// Process data ...
}

void memory_try()
{
	uint8_t *char_ptr = (uint8_t *)malloc(512);
	for (int i = 0; i < 512; ++i) {
		char_ptr[i] = 2;
	}
}

int main() {

	HAL_Init();
	/* Initialize system and peripheral clocks */
	SystemClock_Config();

#ifndef MY_DEBUG
	if (clk_window_Show() != APP_OK) {
		error_handler();
	}

	RTC_TimeTypeDef rtcTime;
	if (clk_window_ReadUserInput(&rtcTime) != APP_OK) {
		error_handler();
	}

	if (rtc_Init() != HAL_OK) {
		error_handler();
	}

	if (rtc_SetTime(&rtcTime) != APP_OK) {
		error_handler();
	}

	if (interval_window_Show() != APP_OK) {
		error_handler();
	}
#endif


	//BSP_LCD_Init();
	//BSP_LCD_Clear(LCD_COLOR_BLACK);

	/*if (temp_sensor_Init() == 0) {
		error_handler();
	}
	temp_sensor_Init();
	temp_sensor_debug();*/

	interval_frame_test();
	//memory_try();

	volatile int a = 0;
	while (1) {
		a++;
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 72000000
 *            HCLK(Hz)                       = 72000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 25000000
 *            HSE PREDIV1                    = 5
 *            HSE PREDIV2                    = 5
 *            PLL2MUL                        = 8
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef clkinitstruct = { 0 };
	RCC_OscInitTypeDef oscinitstruct = { 0 };

	/* Configure PLLs ------------------------------------------------------*/
	/* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
	/* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
	/* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
	oscinitstruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
	oscinitstruct.PLL2.PLL2State = RCC_PLL2_ON;
	oscinitstruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
	oscinitstruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
	if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2) != HAL_OK) {
		/* Initialization Error */
		while (1)
			;
	}
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

