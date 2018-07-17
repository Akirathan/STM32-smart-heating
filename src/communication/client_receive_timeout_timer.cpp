/**
 * @file client_receive_timeout_timer.cpp
 * @author mayfa
 * @date Jul 15, 2018
 */

#include "client_receive_timeout_timer.hpp"
#include "stm32f1xx_hal.h" // For HAL_GetTick
#include "client.hpp" // For Client::handleError
#include "settings.hpp" // For DEVICE_ID
#include "tcp_driver.hpp" // For TcpDriver::isLinkUp

bool     ClientReceiveTimeoutTimer::started = false;
uint32_t ClientReceiveTimeoutTimer::startedTicks = 0;

void ClientReceiveTimeoutTimer::start()
{
	started = true;
	startedTicks = HAL_GetTick();
}

void ClientReceiveTimeoutTimer::checkTimeout()
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




