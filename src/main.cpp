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
	SetIntervalFrame intv_fr;

	intv_fr.passControl();
	std::vector<IntervalFrameData> data_vec = intv_fr.getData();

	// Process data
	EEPROM& eeprom = EEPROM::getInstance();
	eeprom.save(data_vec);

	std::vector<IntervalFrameData> data_vec_copy;
	eeprom.load(data_vec_copy);
}

void mainframe_test()
{
	RTC_TimeTypeDef rtc_time = {0, 0, 0};

	// Init rtc
	RTCController& rtc = RTCController::getInstance();
	rtc.setTime(&rtc_time);

	MainFrame mainframe;
	mainframe.passControl();
}

void static_time_window_test()
{
	/* Clock setting */
	ClkFrame clk_frame;
	RTC_TimeTypeDef rtc_time = clk_frame.passControl();
	// Save time into rtc
	RTCController::getInstance().setTime(&rtc_time);

	/* static_time_window initialization */
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	StaticTimeWindow time_window{Coord{10,BSP_LCD_GetYSize()/2}};
	time_window.draw();

	// rtc keeps calling back time_window, and time_window
	// keeps updating seconds value.
	while (true) ;
}

void main_test()
{
	/* Clock setting */
	ClkFrame clk_frame;
	RTC_TimeTypeDef rtc_time = clk_frame.passControl();
	// Save time into rtc
	RTCController::getInstance().setTime(&rtc_time);

	/* Interval setting */
	std::vector<IntervalFrameData> interval_vec;
	EEPROM& eeprom = EEPROM::getInstance();

	if (eeprom.isEmpty()) {
		SetIntervalFrame intv_fr;
		intv_fr.passControl();
		interval_vec = intv_fr.getData();

		eeprom.save(interval_vec);
	}
	else {
		eeprom.load(interval_vec);
	}

	/* Main frame */
	MainFrame mainframe;
	mainframe.passControl();
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

/**
 * Pin set as input pullup is read as set.
 */
void gpio_test()
{
	// Enable clock
	__HAL_RCC_GPIOE_CLK_ENABLE();

	// Set PE3 as input pullup
	GPIO_InitTypeDef gpio;
	gpio.Pin = GPIO_PIN_3;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &gpio);

	GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3); // set expected
}

int main()
{

	HAL_Init();
	/* Initialize system and peripheral clocks */
	SystemClock_Config();

#ifndef MY_DEBUG
	ClkFrame ClkFrame;
	RTC_TimeTypeDef rtc_time = ClkFrame.passControl();

	// Enable LED for second_it
	BSP_LED_Init(LED_BLUE);

	RTCController& RTCController = RTCController.getInstance(); // instantiation of rtc

	if (RTCController.setTime(&rtc_time) != APP_OK) {
		error_handler();
	}

#endif

	main_test();
	//mainframe_test();

	/*BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Hello1", CENTER_MODE);
	BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize()/2, LINE(2), (uint8_t *)"Hello2", LEFT_MODE);*/

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

