/**
 * @file relay.cpp
 * @author Pavel Marek
 * @date 31.6.2017
 */

#include "relay.hpp"

Relay& Relay::getInstance()
{
	static Relay instance;
	return instance;
}

void Relay::switchOn()
{
	HAL_GPIO_WritePin(RELAY_DATA_GPIO_Port, RELAY_DATA_Pin, GPIO_PIN_SET);
}

void Relay::switchOff()
{
	HAL_GPIO_WritePin(RELAY_DATA_GPIO_Port, RELAY_DATA_Pin, GPIO_PIN_RESET);
}
