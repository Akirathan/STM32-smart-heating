#include "main.hpp"

#define MY_DEBUG

/* Private function prototypes */
void SystemClock_Config(void);

void error_handler() {
	BSP_LED_Init(LED_RED);
	BSP_LED_On(LED_RED);

	while (true) ;
}

void intervalframe_test()
{
	intervalframe intv_fr;
	std::vector<intervalframe_data> data_vec = intv_fr.pass_control();

	// Process data
	eeprom& eeprom = eeprom.get_instance();
	eeprom.save(data_vec);

	std::vector<intervalframe_data> data_vec_copy;
	eeprom.load(data_vec_copy);
}

void mainframe_test()
{
	RTC_TimeTypeDef rtc_time = {0, 0, 0};

	// Init rtc
	rtc& rtc = rtc::get_instance();
	rtc.set_time(&rtc_time);

	mainframe mainframe;
	mainframe.pass_control();
}

void static_time_window_test()
{
	/* Clock setting */
	clk_frame clk_frame;
	RTC_TimeTypeDef rtc_time = clk_frame.pass_control();
	// Save time into rtc
	rtc::get_instance().set_time(&rtc_time);

	/* static_time_window initialization */
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	static_time_window time_window{coord{10,BSP_LCD_GetYSize()/2}};
	time_window.draw();

	// rtc keeps calling back time_window, and time_window
	// keeps updating seconds value.
	while (true) ;
}

void main_test()
{
	/* Clock setting */
	clk_frame clk_frame;
	RTC_TimeTypeDef rtc_time = clk_frame.pass_control();
	// Save time into rtc
	rtc::get_instance().set_time(&rtc_time);

	/* Interval setting */
	std::vector<intervalframe_data> interval_vec;
	eeprom& eeprom = eeprom.get_instance();

	if (eeprom.is_empty()) {
		intervalframe intv_fr;
		interval_vec = intv_fr.pass_control();

		eeprom.save(interval_vec);
	}
	else {
		eeprom.load(interval_vec);
	}

	/* Main frame */


}

uint8_t memory_try()
{
	uint8_t* heap_byte = (uint8_t *)malloc(1);
	uint8_t stack_byte = 32;
	return stack_byte;
}

void write_try()
{
	print("ab\n");
	print("cd");
}

void eeprom_try()
{
	uint8_t buff[8] = {2,2,2,2,2,2,2,2};
	uint8_t rx_buff[8];
	uint32_t read_bytes = 8;

	BSP_EEPROM_SelectDevice(BSP_EEPROM_M24C64_32);

	if (BSP_EEPROM_Init() != EEPROM_OK) {
		error_handler();
	}

	if (BSP_EEPROM_WriteBuffer(buff, 0, 8) != EEPROM_OK) {
		error_handler();
	}

	if (BSP_EEPROM_ReadBuffer(rx_buff, 0, &read_bytes) != EEPROM_OK) {
		error_handler();
	}
}

int main() {

	HAL_Init();
	/* Initialize system and peripheral clocks */
	SystemClock_Config();

#ifndef MY_DEBUG
	clk_frame clk_frame;
	RTC_TimeTypeDef rtc_time = clk_frame.pass_control();

	// Enable LED for second_it
	BSP_LED_Init(LED_BLUE);

	rtc& rtc = rtc.get_instance(); // instantiation of rtc

	if (rtc.set_time(&rtc_time) != APP_OK) {
		error_handler();
	}

#endif

	// TODO: temp_sensor to class?
	if (temp_sensor_Init() == 0) {
		error_handler();
	}
	temp_sensor_debug();

	//main_test();
	//mainframe_test();
	//static_time_window_test();


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

