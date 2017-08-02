/*
 * rtc.cpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include <rtc_controller.hpp>

#define ENABLE_SEC_INTERRUPT

/**
 * Second interrupt handler.
 */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef* hrtc)
{
	RTCController& rtc = RTCController::getInstance();
	rtc.update();
}

RTCController& RTCController::getInstance()
{
	static RTCController instance;
	return instance;
}

/**
 * Initializes RTC peripheral. In case of wakeup from power reset mode,
 * RTC configuration is still saved in backup domain. This constructor
 * does not care about previous RTC initializations, and overwrites
 * this configuration into backup domain.
 */
RTCController::RTCController()
{
	RCC_OscInitTypeDef oscilator;

	// Enable the LSE oscilator.
	HAL_RCC_GetOscConfig(&oscilator); // Get current state of oscillators
	oscilator.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	// The PLLs will not be really disabled,
	// we just dont want the HAL_RCC_OscConfig
	// function to disable the PLL so it can
	// configure them.
	oscilator.PLL.PLLState = RCC_PLL_NONE;
	oscilator.PLL2.PLL2State = RCC_PLL2_NONE;
	oscilator.LSEState = RCC_LSE_ON;
	oscilator.LSIState = RCC_LSI_OFF;
	if (HAL_RCC_OscConfig(&oscilator) != HAL_OK) {
		//TODO: return HAL_ERROR;
	}

	// Configure the clock source for RTC.
	RCC_PeriphCLKInitTypeDef periphClock;
	periphClock.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periphClock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClock) != HAL_OK) {
		// TODO: return HAL_ERROR;
	}

	__HAL_RCC_RTC_ENABLE(); // Enable RTC clock

	// Initialize RTC, configure prescaler, etc.
	handle.Instance = RTC;
	handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	HAL_RTC_Init(&handle);

#ifdef ENABLE_SEC_INTERRUPT
	// Configure the NVIC for RTC global interrupt
	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);

	// Enable the RTC global Interrupt
	HAL_NVIC_EnableIRQ(RTC_IRQn);

	// Enable second interrupt
	HAL_RTCEx_SetSecond_IT(&handle);
#else
	HAL_RTCEx_DeactivateSecond(&handle);
#endif
}

AppStatus_TypeDef RTCController::setTime(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_SetTime(&handle, time, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		timeSet = true;
		return APP_OK;
	}
}


AppStatus_TypeDef RTCController::getTime(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_GetTime(&handle, time, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		return APP_OK;
	}
}

bool RTCController::isTimeSet() const
{
	return timeSet;
}

RTC_HandleTypeDef& RTCController::getHandle()
{
	return handle;
}


/**
 * Called by second interrupt handler.
 */
void RTCController::update()
{
	if (timeSet == false) {
		return ;
	}

	RTC_TimeTypeDef rtc_time;
	getTime(&rtc_time);

	// Second overflow. Call all registered "functions".
	for (ISecCallback* sec_callback: secondCallbackVec) {
		sec_callback->secCallback();
	}

	// Minute overflow.
	if (rtc_time.Seconds == 0) {
		// Call all registered "functions"
		for (IMinCallback* min_callback : minuteCallbackVec) {
			min_callback->minCallback();
		}
	}

	// Hour overflow
	if (rtc_time.Minutes == 0) {
		// ...
	}
}

void RTCController::registerMinuteCallback(IMinCallback* min_callback)
{
	minuteCallbackVec.push_back(min_callback);
}

void RTCController::registerSecondCallback(ISecCallback* sec_callback)
{
	secondCallbackVec.push_back(sec_callback);
}

/**
 * Find pointer inside vector and deletes it.
 */
void RTCController::unregisterSecondCallback(ISecCallback* sec_callback)
{
	for (auto it = secondCallbackVec.begin(); it != secondCallbackVec.end(); ++it) {
		if (*it == sec_callback) {
			secondCallbackVec.erase(it);
			break;
		}
	}
}

/**
 * Find pointer inside vector and deletes it.
 */
void RTCController::unregisterMinuteCallback(IMinCallback *min_callback)
{
	for (auto it = minuteCallbackVec.begin(); it != minuteCallbackVec.end(); ++it) {
		if (*it == min_callback) {
			minuteCallbackVec.erase(it);
			break;
		}
	}
}
