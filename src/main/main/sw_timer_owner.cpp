/**
 * @file sw_timer_owner.cpp
 * @author Pavel Marek
 * @date Jul 7, 2018
 */

#include "sw_timer_owner.hpp"
#include "application.hpp" // For Application::registerForPolling
#include "stm32f1xx_hal.h" // For HAL_GetTick

SwTimerOwner::SwTimerOwner() :
	SwTimerOwner(0)
{ }

SwTimerOwner::SwTimerOwner(unsigned int timeout) :
	timeoutMs(timeout),
	started(false),
	startedTicks(0)
{
	Application::registerSwTimerOwnerForPolling(this);
}

/**
 * @brief Polls this SwTimerOwner for timeout.
 * This method is called periodically from @ref Application.
 */
void SwTimerOwner::poll()
{
	if (!started) {
		return;
	}

	if (HAL_GetTick() - startedTicks >= timeoutMs) {
		startedTicks = HAL_GetTick();
		timeout();
	}
}

/**
 * Starts this SwTimer, may be called many times (after stop method).
 */
void SwTimerOwner::startSwTimer()
{
	startedTicks = HAL_GetTick();
	started = true;
}

/**
 * Stops this SwTimer - no timeout callbacks will be called.
 */
void SwTimerOwner::stopSwTimer()
{
	started = false;
	startedTicks = 0;
}

