/**
 * @file rtc_controller.cpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#include <rtc_controller.hpp>

// Uncomment this to disable second interrupt.
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
 * @brief Initializes RTC peripheral.
 *
 * In case of wakeup from power reset mode, RTC configuration is still saved
 * in backup domain. This constructor does not care about previous RTC
 * initializations, and overwrites this configuration into backup domain.
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
		//TODO HAL_ERROR;
	}

	// Configure the clock source for RTC.
	RCC_PeriphCLKInitTypeDef periphClock;
	periphClock.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periphClock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClock) != HAL_OK) {
		// TODO HAL_ERROR;
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

/**
 * @brief Sets current time into RTC.
 *
 * Wrapper for @ref HAL_RTC_SetTime function.
 */
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

/**
 * @brief Gets current time from RTC.
 *
 * Wrapper for @ref HAL_RTC_GetTime function.
 */
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

/**
 * @brief Returns HAL handle for RTC.
 *
 * This method is called from @ref RTC_IRQHandler function.
 */
RTC_HandleTypeDef& RTCController::getHandle()
{
	return handle;
}


/**
 * @brief Called every second by interrupt handler.
 *
 * Calls registered objects' callbacks.
 *
 * @note Called by @ref RTC_IRQHandler function.
 */
void RTCController::update()
{
	if (timeSet == false) {
		return;
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

/**
 * @brief Registers an object for minute callback.
 */
void RTCController::registerMinuteCallback(IMinCallback* min_callback)
{
	minuteCallbackVec.push_back(min_callback);
}

/**
 * @brief Registers an object for second callback.
 */
void RTCController::registerSecondCallback(ISecCallback* sec_callback)
{
	secondCallbackVec.push_back(sec_callback);
}

/**
 * @brief Unregisters an object from second callback.
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
 * @brief Unregisters an object from minute callback.
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
