/*
 * relay.c
 *
 *  Created on: 31 Jan 2017
 *      Author: Mayfa
 */

#include "relay.hpp"

/**
 * Init the relay peripheral. Supply the power to the relay.
 */
void relay_Init() {
	/* Init clock for data pin and for power pin */
	if (RELAY_DATA_GPIOPORT == GPIOA || RELAY_POWER_GPIOPORT == GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (RELAY_DATA_GPIOPORT == GPIOB || RELAY_POWER_GPIOPORT == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (RELAY_DATA_GPIOPORT == GPIOC || RELAY_POWER_GPIOPORT == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (RELAY_DATA_GPIOPORT == GPIOD || RELAY_POWER_GPIOPORT == GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (RELAY_DATA_GPIOPORT == GPIOE || RELAY_POWER_GPIOPORT == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}

	/* Set the relay as output push-pull */
	GPIO_InitTypeDef GPIO_Init;

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Pin = RELAY_DATA_GPIOPIN;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(RELAY_DATA_GPIOPORT, &GPIO_Init);

	/* Init the power connection */
	GPIO_Init.Pin = RELAY_POWER_GPIOPIN;
	HAL_GPIO_Init(RELAY_POWER_GPIOPORT, &GPIO_Init);
	// Toggle the power
	HAL_GPIO_WritePin(RELAY_POWER_GPIOPORT, RELAY_POWER_GPIOPIN, GPIO_PIN_SET);
}

void relay_SwitchOn() {
	HAL_GPIO_WritePin(RELAY_DATA_GPIOPORT, RELAY_DATA_GPIOPIN, GPIO_PIN_SET);
}

void relay_SwitchOff() {
	HAL_GPIO_WritePin(RELAY_DATA_GPIOPORT, RELAY_DATA_GPIOPIN, GPIO_PIN_RESET);
}
