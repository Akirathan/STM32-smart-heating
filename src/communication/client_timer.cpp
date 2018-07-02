/**
 * @file client_timer.cpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#include "client_timer.hpp"
#include "stm32f1xx_hal.h" // For HAL_GetTick
#include "client.hpp" // For Client::startCycle

bool     ClientTimer::started = false;
uint32_t ClientTimer::startedTicks = 0;

void ClientTimer::start()
{
	started = true;
	startedTicks = HAL_GetTick();
}

/**
 * @brief Checks if timer expired.
 *
 * This function can be called even if ClientTimer is not started.
 */
void ClientTimer::checkTimeout()
{
	if (!started) {
		return;
	}

	if (HAL_GetTick() - startedTicks >= TIMEOUT_MS) {
		started = false;
		startedTicks = 0;
		if (Client::isConnected()) {
			Client::startCycle();
		}
	}
}
