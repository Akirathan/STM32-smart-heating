/**
 * @file main.cpp
 * @author Pavel Marek
 * @brief This file contains main login of the application.
 *
 * It also contains clock configuration function provided by STM.
 */

#include "main.hpp"

void SystemClock_Config();

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
	clk_frame.passControl();
	RTC_TimeTypeDef rtc_time = clk_frame.getTime();
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
	// Clock setting.
	ClkFrame clk_frame;
	clk_frame.passControl();
	RTC_TimeTypeDef rtc_time = clk_frame.getTime();
	// Save time into rtc.
	RTCController::getInstance().setTime(&rtc_time);

	// Interval setting.
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

	// Main frame.
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
	IO::print("ab\n");
	IO::print("cd");
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

/*static err_t net_tcp_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{

}*/

/*static err_t net_tcp_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{

}*/

/**
 * Receives packets in while loop.
 */
/*void net_try()
{
	struct netif net;

	lwip_init();

	// Init IP address.
	struct ip_addr ip;
	struct ip_addr netmask;
	struct ip_addr gw;
	IP4_ADDR(&ip, 198, 0, 0, 2);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 198, 0, 0, 2);

	// Config.
	netif_add(&net, &ip, &netmask, &gw, NULL, &ethernetif_init,
			(void (*) (netif *))&ethernetif_input);
	netif_set_default(&net);
	netif_set_up(&net);
	netif_set_link_callback(&net, ethernetif_update_config);

	// Set TCP.
	struct tcp_pcb * pcb;
	pcb = tcp_new();
	err_t err = tcp_bind(pcb, IP_ADDR_ANY, 4567);

	if (err == ERR_OK) {
		pcb = tcp_listen(pcb);
	}

	while (true) {
		ethernetif_input(&net);
	}
}*/

/*void fat_try(bool formatted, const std::string& fname)
{
	if (FATFS_LinkDriver(&SD_Driver, "") != 0) {
		error_handler();
	}

	// Mount.
	FATFS fs;
	FRESULT res;
	if ((res = f_mount(&fs, "", 0)) != F_OK) {
		error_handler();
	}

	// Format.
	if (!formatted) {
		if ((res = f_mkfs("", 0, 0)) != F_OK) {
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
}*/

extern "C" {
	extern int cube_main();
}

int main()
{
	cube_main();

	//fat_try(false, "file.txt");
	//net_try();
	main_test();

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

