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

/* Testing includes (can be removed in release) */
#include "char_stream_test.hpp"
#include "response_buffer_test.hpp"

#include "crypto.h"


void SystemClock_Config();
static void board_init();

void error_handler() {
	BSP_LED_Init(LED_RED);
	BSP_LED_On(LED_RED);

	while (true) ;
}

void static_time_window_test()
{
	/* Clock setting */
	ClkFrame clk_frame;
	clk_frame.passControl();
	Time::Time time = clk_frame.getTime();
	// Save time into rtc
	RTCController::getInstance().setTime(time);

	/* static_time_window initialization */
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	StaticTimeWindow time_window{Coord{10,BSP_LCD_GetYSize()/2}};
	time_window.draw();

	// rtc keeps calling back time_window, and time_window
	// keeps updating seconds value.
	while (true) ;
}

/**
 * Reads whole contents of EEPROM - debugging.
 */
void read_eeprom()
{
	IntervalFrameData data[INTERVALS_NUM];
	size_t data_size = 0;
	uint32_t timestamp = 0;
	bool time_synced = false;
	EEPROM &eeprom = EEPROM::getInstance();

	eeprom.load(data, &data_size, &timestamp, &time_synced);
}

void fat_try(bool formatted, const std::string& fname)
{
	char path[5];
	if (FATFS_LinkDriver(&SD_Driver, path) != 0) {
		error_handler();
	}

	// Mount.
	FATFS fs;
	FRESULT res;
	if ((res = f_mount(&fs, path, 0)) != F_OK) {
		error_handler();
	}

	// Format.
	if (!formatted) {
		if ((res = f_mkfs(path, 0, 0)) != F_OK) {
			error_handler();
		}
	}

	// Open.
	FIL file;
	if ((res = f_open(&file, fname.c_str(), FA_READ)) != F_OK) {
		error_handler();
	}

	// Read.
	uint8_t rxbuff[512];
	UINT br;
	f_read(&file, rxbuff, 512, &br);

	// Fill wrbuffer.
	uint8_t wrbuff[512];
	for (int i = 0; i < 512; ++i) {
		wrbuff[i] = i;
	}

	// Write.
	f_write(&file, wrbuff, 512, &br);
}

void tests()
{
	CharStreamTest test;
	test.runAll();
}

extern "C" {
	extern int cube_main();
}

void des_encrypt(const uint8_t *key, const uint8_t *in_buff, const int32_t in_len,
		uint8_t *out_buff, int32_t *out_len)
{
	DESECBctx_stt des_ctx;
	des_ctx.mFlags = E_SK_DEFAULT;

	int32_t state = DES_ECB_Encrypt_Init(&des_ctx, key, nullptr);
	rt_assert(state == DES_SUCCESS, "DES init failed");

	out_buff[8] = {0};
	int32_t tmp_out_len = 0;
	state = DES_ECB_Encrypt_Append(&des_ctx, in_buff, in_len, out_buff, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "DES encrypt append failed");

	*out_len = tmp_out_len;

	state = DES_ECB_Encrypt_Finish(&des_ctx, out_buff + tmp_out_len, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "Des_ECB_Encrypt_Finish failed");

	*out_len += tmp_out_len;
}

void des_decrypt(const uint8_t *key, const uint8_t *in_buff, const int32_t in_len,
		uint8_t *out_buff, int32_t *out_len)
{
	DESECBctx_stt des_ctx;
	des_ctx.mFlags = E_SK_DEFAULT;

	int32_t state = DES_ECB_Decrypt_Init(&des_ctx, key, nullptr);
	rt_assert(state == DES_SUCCESS, "DES_ECB_Decrypt_init failed");

	int32_t tmp_out_len = 0;
	state = DES_ECB_Decrypt_Append(&des_ctx, in_buff, in_len, out_buff, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "DES_ECB_Decrypt_Append failed");

	*out_len = tmp_out_len;

	state = DES_ECB_Decrypt_Finish(&des_ctx, out_buff + tmp_out_len, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "DES_ECB_Decrypt_Finish failed");

	*out_len += tmp_out_len;
}

int main()
{
	cube_main();
	board_init();


	const uint8_t des_key[8] = {0x01, 0x23, 0x45, 0x67, 0x78, 0x9A, 0xBC, 0xDE};
	const uint8_t *plain_text = "nazdar!\0";
	const int32_t plaint_text_len = 8;

	uint8_t enc_text[8] = {0};
	int32_t enc_text_len = 0;
	des_encrypt(des_key, plain_text, plaint_text_len, enc_text, &enc_text_len);

	uint8_t dec_text[8] = {0};
	int32_t dec_text_len = 0;
	des_decrypt(des_key, enc_text, enc_text_len, dec_text, &dec_text_len);

	volatile int a = 0;
	while (1) {
		a++;
	}
}

static void board_init()
{
	// CRC peripheral is used by crypto library.
	__HAL_RCC_CRC_CLK_ENABLE();

	LCD::init();

	lwip_init();
	TcpDriver::init(192, 168, 0, 1, 8000);
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

