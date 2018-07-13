/**
 * @file client_error_timer.cpp
 * @author mayfa
 * @date Jul 13, 2018
 */

#include "client_error_timer.hpp"
#include "stm32f1xx_hal.h" // For HAL_GetTick
#include "client.hpp" // For Client::startCycle
#include "settings.hpp" // For DEVICE_ID
#include "tcp_driver.hpp" // For TcpDriver::isLinkUp

bool     ClientErrorTimer::started = false;
uint32_t ClientErrorTimer::startedTicks = 0;

void ClientErrorTimer::start()
{
	started = true;
	startedTicks = HAL_GetTick();
}

/**
 * Try to start the communication cycle from the beggining ie. call
 * @ref Client::sendConnectReq
 */
void ClientErrorTimer::checkTimeout()
{
	if (!started) {
		return;
	}

	if (HAL_GetTick() - startedTicks >= TIMEOUT_MS) {
		started = false;
		startedTicks = 0;
		if (TcpDriver::isLinkUp()) {
			Client::sendConnectReq(DEVICE_ID);
		}
	}
}

