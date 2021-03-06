/**
 * @file one_wire.cpp
 * @author Pavel Marek
 * @date 26.12.2016
 */

#include "one_wire.hpp"

namespace OneWire {

// PE1
#define CLK_PIN		GPIO_PIN_1
#define CLK_PORT	GPIOE

// Private functions
static uint32_t init_DWT();
static inline void set_transmit();
static inline void set_receive();
static void master_release_bus();
static void master_pull_bus_low();
static int slave_pull_bus_low();
static inline void wait(uint32_t micros);

// Private variables
static bool initialized = false;

/**
 * @brief Inits the one-wire peripheral.
 */
uint32_t init()
{
	if (initialized) {
		return 0;
	}

	// TODO: initialization is done in cube-mx
	initialized = true;
	// Init DWT
	return init_DWT();
}

/**
 * Implemented with DWT.
 */
static inline void wait(__IO uint32_t micros)
{
	uint32_t start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	micros *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - start) < micros)
		;
}

/**
 * DWT peripheral is necessary for CPU tick counter. The wait function is
 * most accurate with DWT implementation.
 *
 * TODO: use macros from stm32f107xc.h instead
 * 	of pure bits, ie. make this function more
 * 	readable.
 */
static uint32_t init_DWT()
{
	uint32_t c;

	/* Enable TRC (DWT) */
	CoreDebug->DEMCR &= ~0x01000000;
	CoreDebug->DEMCR |= 0x01000000; // Set TRCENA bit to 1

	/* Enable CYCCNT counter */
	DWT->CTRL &= ~0x00000001;
	DWT->CTRL |= 0x00000001;

	/* Reset counter */
	DWT->CYCCNT = 0;

	/* Check if DWT has started */
	c = DWT->CYCCNT;

	/* 2 dummys */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	//__NOP();
	//__NOP();

	/* Return difference, if result is zero, DWT has not started */
	return (DWT->CYCCNT - c);
}

/**
 * Sets the master for transmit, ie. set PE4 as output push-pull
 * with medium speed (10 MHz), without resistors.
 * Note that because of efficiency this function directly modifies
 * registers.
 */
static inline void set_transmit()
{
	// PE4 <- output, medium speed, nopull
	MODIFY_REG(TEMP_DATA_GPIO_Port->CRL,
			(GPIO_CRL_CNF0 << TEMP_DATA_Pin) | (GPIO_CRL_MODE0_1 << TEMP_DATA_Pin),
			(GPIO_CRL_MODE0_0 << TEMP_DATA_Pin));
}

/**
 * Sets the master for receive, ie. sets PE4 as input with
 * weak pull-up resistor.
 * Note that because of efficiency this function directly modifies
 * registers.
 */
static inline void set_receive()
{
	// PE4 <- input, medium speed, pullup
	MODIFY_REG(TEMP_DATA_GPIO_Port->CRL,
			(GPIO_CRL_CNF0_0 << TEMP_DATA_Pin) | (GPIO_CRL_MODE0 << TEMP_DATA_Pin),
			(GPIO_CRL_CNF0_1 << TEMP_DATA_Pin));

	SET_BIT(TEMP_DATA_GPIO_Port->ODR, TEMP_DATA_Pin);
}

/**
 * @brief Resets the slave device.
 */
void reset()
{
	master_pull_bus_low();
	wait(490);
}

/**
 * @brief Inits communication with one-wire device.
 * @return: 0 when there was no response
 * 			1 when there was response.
 */
int init_communication()
{
	int response = 0;

	/* Master Tx reset pulse */
	reset();

	/* Wait 60-240 microseconds for the response */
	// Release the 1-wire bus
	master_release_bus();
	wait(70);
	// Check the response
	if (slave_pull_bus_low()) {
		response = 1;
	}
	// Wait until timeout
	wait(410);

	return response;
}

/**
 * @brief Writes a byte to the slave in LSB order.
 */
void write_byte(uint8_t byte)
{
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN); //

	for (int i = 0; i < 8; i++) {
		// Least significant bit is first
		write_bit(byte & 0x01);
		byte >>= 1;
	}
}

void write_bit(uint8_t bit)
{
	master_pull_bus_low();

	if (bit == 0) {
		/* Keep the bus low for entire write time slot */
		wait(70);
		master_release_bus();
	}
	else if (bit == 1) {
		/* Release the bus within 15 microseconds */
		wait(10);
		// Release the bus
		master_release_bus();
		wait(55);
	}

	// Recovery time
	wait(2);
}

uint8_t read_byte()
{
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN); //

	uint8_t byte = 0x00;

	for (int i = 0; i < 8; i++) {
		byte >>= 1;
		byte |= (read_bit() << 7);
	}

	return byte;
}

uint8_t read_bit()
{
	uint8_t bit = 0x00;
	// Master initiates read time slot by pulling
	// the bus low for a minimum of 1 us.
	master_pull_bus_low();
	wait(3);

	master_release_bus();
	// Master has to sample the bus state within 15 us.
	wait(9);
	if (slave_pull_bus_low()) {
		bit = 0x00;
	}
	else {
		bit = 0x01;
	}

	// Wait to complete 60us period
	wait(60);
	return bit;
}

/**
 * @brief Releases the bus.
 */
static inline void master_release_bus()
{
	set_receive();
}

/**
 * @brief Pulls the bus low.
 */
static inline void master_pull_bus_low()
{
	set_transmit();
	HAL_GPIO_WritePin(TEMP_DATA_GPIO_Port, TEMP_DATA_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Checks whether the slave device pulled the bus low.
 * @return 1 if the bus is pulled low by the slave, 0 otherwise.
 */
static inline int slave_pull_bus_low()
{
	set_receive();
	if (HAL_GPIO_ReadPin(TEMP_DATA_GPIO_Port, TEMP_DATA_Pin) == GPIO_PIN_RESET) {
		return 1;
	}
	else {
		return 0;
	}
}
} // namespace one_wire
