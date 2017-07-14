/*
 * one_wire.c
 *
 *  Created on: 26. 12. 2016
 *      Author: Mayfa
 */

#include <one_wire.hpp>

#define CLK_PIN		GPIO_PIN_1
#define CLK_PORT	GPIOE

/* Private functions */
static uint32_t one_wire_InitDWT();
static inline void one_wire_SetTransmit();
static inline void one_wire_SetReceive();
static void one_wire_MasterReleaseBus();
static void one_wire_MasterPullBusLow();
static int one_wire_SlavePullBusLow();
static int one_wire_SlaveReleaseBus();
static inline void one_wire_WriteBit(uint8_t bit);
static inline void wait(uint32_t micros);
static int test_data(temp_sensor_data_t* data);


/**
 * Init the one-wire peripheral.
 */
uint32_t one_wire_Init() {
	// Init clock
	if (TEMP_SENSOR_DATA_GPIOPORT == GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (TEMP_SENSOR_DATA_GPIOPORT == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (TEMP_SENSOR_DATA_GPIOPORT == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (TEMP_SENSOR_DATA_GPIOPORT == GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (TEMP_SENSOR_DATA_GPIOPORT == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}

	// Init CLK debug
	GPIO_InitTypeDef init;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pin = CLK_PIN;
	init.Speed = GPIO_SPEED_FREQ_MEDIUM;
	init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(CLK_PORT, &init);

	// Init DWT
	return one_wire_InitDWT();
}

/**
 * Calibrate the wait function with this
 * function.
 */
uint32_t one_wire_TimeTest(uint32_t microsecs) {
	uint32_t beginTicks = HAL_GetTick();
	wait(microsecs);
	uint32_t endTicks = HAL_GetTick();
	return endTicks - beginTicks;
}

/**
 * Implemented with DWT.
 */
static inline void wait(__IO uint32_t micros) {
	uint32_t start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	micros *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - start) < micros);
}

/**
 * DWT peripheral is necessary for CPU
 * tick counter. The wait function is
 * most accurate with DWT implementation.
 */
static uint32_t one_wire_InitDWT() {
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

	/* Return difference, if result is zero, DWT has not started */
	return (DWT->CYCCNT - c);
}

/**
 * Sets the master for transmit.
 */
static inline void one_wire_SetTransmit() {
	GPIO_InitTypeDef GPIO_Init;

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Pin = TEMP_SENSOR_DATA_GPIOPIN;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(TEMP_SENSOR_DATA_GPIOPORT, &GPIO_Init);
}

static inline void one_wire_SetReceive() {
	GPIO_InitTypeDef GPIO_Init;

	// Set the GPIO pin as input with weak pull-up
	// resistor enabled.
	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Pin = TEMP_SENSOR_DATA_GPIOPIN;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(TEMP_SENSOR_DATA_GPIOPORT, &GPIO_Init);
}

/**
 * Test written data to temperature sensor.
 */
static int test_data(temp_sensor_data_t* data) {
	temp_sensor_data_t data2;

	temp_sensor_ReadData(&data2);

	if (data2.TH == data->TH && data2.TL == data->TL && data2.CFG == data->CFG
			&& data2.TEMP_LSB == data->TEMP_LSB && data2.TEMP_MSB == data->TEMP_MSB)
	{
		return 1;
	}
	else {
		return 0;
	}
}

void one_wire_Reset() {
	one_wire_MasterPullBusLow();
	wait(490);
}

void one_wire_debug() {
	one_wire_MasterReleaseBus();
	/*wait(300);
	one_wire_MasterPullBusLow();
	wait(300);
	one_wire_MasterReleaseBus();
	wait(300);
	one_wire_MasterPullBusLow();*/
}

/**
 * @return: 0 when there was no response
 * 			1 when there was response.
 */
int one_wire_InitCommunication() {
	int response = 0;

	/* Master Tx reset pulse */
	one_wire_MasterPullBusLow();
	// Wait at least 480 microseconds
	wait(480);

	/* Wait 60-240 microseconds for the response */
	// Release the 1-wire bus
	one_wire_MasterReleaseBus();
	wait(70);
	// Check the response
	if (one_wire_SlavePullBusLow()) {
		response = 1;
	}
	// Wait until timeout
	wait(410);

	return response;
}

/**
 * Write a byte to the slave in LSB order.
 */
void one_wire_WriteByte(uint8_t byte) {
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN); //

	for (int i = 0; i < 8; i++) {
		// Least significant bit is first
		one_wire_WriteBit(byte & 0x01);
		byte >>= 1;
	}
}

/**
 * Each write/read time slot is at least 60 us long.
 */
static inline void one_wire_WriteBit(uint8_t bit) {
	one_wire_MasterPullBusLow();

	if (bit == 0) {
		/* Keep the bus low for entire write time slot */
		wait(60);
		one_wire_MasterReleaseBus();
	}
	else if (bit == 1) {
		/* Release the bus within 15 microseconds */
		wait(10);
		// Release the bus
		one_wire_MasterReleaseBus();
		wait(55);
	}

	// Recovery time
	wait(2);
}

uint8_t one_wire_ReadByte() {
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN); //

	uint8_t byte = 0x00;

	for (int i = 0; i < 8; i++) {
		byte >>= 1;
		byte |= (one_wire_ReadBit() << 7);
	}

	return byte;
}

uint8_t one_wire_ReadBit() {
	uint8_t bit = 0x00;
	// Master initiates read time slot by pulling
	// the bus low for a minimum of 1 us.
	one_wire_MasterPullBusLow();
	wait(3);

	one_wire_MasterReleaseBus();
	// Master has to sample the bus state
	// within 15 us.
	wait(9);
	if (one_wire_SlavePullBusLow()) {
		bit = 0x00;
	}
	else {
		bit = 0x01;
	}

	// Wait to complete 60us period
	wait(50);
	return bit;
}

/**
 * Master releases the bus.
 */
static inline void one_wire_MasterReleaseBus() {
	one_wire_SetReceive();
}

/**
 * Master pulls the bus low.
 */
static inline void one_wire_MasterPullBusLow() {
	one_wire_SetTransmit();
	HAL_GPIO_WritePin(TEMP_SENSOR_DATA_GPIOPORT, TEMP_SENSOR_DATA_GPIOPIN, GPIO_PIN_RESET);
}

/**
 * Returns 1 if the bus is pulled low
 * by the slave and 0 if not.
 */
static inline int one_wire_SlavePullBusLow() {
	one_wire_SetReceive();
	if (HAL_GPIO_ReadPin(TEMP_SENSOR_DATA_GPIOPORT, TEMP_SENSOR_DATA_GPIOPIN) == GPIO_PIN_RESET) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * Checks if slave released the bus.
 */
static inline int one_wire_SlaveReleaseBus() {
	one_wire_SetReceive();
	// Wait a moment so the slave can pull the bus low if he intends to
	wait(5);
	if (HAL_GPIO_ReadPin(TEMP_SENSOR_DATA_GPIOPORT, TEMP_SENSOR_DATA_GPIOPIN) == GPIO_PIN_SET) {
		// The bus is released
		return 1;
	}
	else {
		return 0;
	}
}
