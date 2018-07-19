/**
 * @file main.cpp
 * @author Pavel Marek
 * @brief This file contains main login of the application.
 *
 * It also contains clock configuration function provided by STM.
 */

#include "main.hpp"
#include "application.hpp"
#include "tcp_driver.hpp"

void SystemClock_Config();
static void board_init();

static void eeprom_clear()
{
	EEPROM &eeprom = EEPROM::getInstance();
	eeprom.reset();
	rt_assert(eeprom.isEmpty(), "");
	rt_assert(!eeprom.isKeySet(), "");
}

static void eeprom_fill_intervals()
{
	EEPROM &eeprom = EEPROM::getInstance();

	// Full EEPROM, timestamp = 1, timesynced = false
	IntervalFrameData data[INTERVALS_NUM];
	for (size_t i = 0; i < INTERVALS_NUM; i++) {
		data[i] = IntervalFrameData(i*60 + 60, i*60 + 120, i+10);
	}
	eeprom.save(data, INTERVALS_NUM, 1, false);
}

static void eeprom_read()
{
	EEPROM &eeprom = EEPROM::getInstance();

	DesKey des_key;
	if (eeprom.isKeySet()) {
		des_key = eeprom.loadKey();
	}

	IntervalFrameData data[INTERVALS_NUM];
	size_t data_size = 0;
	bool time_synced = false;
	uint32_t timestamp_load = 0;
	if (!eeprom.isEmpty()) {
		eeprom.load(data, &data_size, &timestamp_load, &time_synced);
	}
}

extern "C" {
	extern int cube_main();
}

int main()
{
	cube_main();
	board_init();

	Application app;
	app.run();
	//eeprom_clear();
}

static void board_init()
{
	// CRC peripheral is used by crypto library.
	__HAL_RCC_CRC_CLK_ENABLE();

	LCD::init();

	lwip_init();
	TcpDriver::init(8000);
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

