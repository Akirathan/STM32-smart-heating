/**
 * @file rtc_controller.cpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#include <rtc_controller.hpp>

extern RTC_HandleTypeDef hrtc; // from cube-mx

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
}

/**
 * @brief Sets current time into RTC.
 *
 * Wrapper for @ref HAL_RTC_SetTime function.
 */
void RTCController::setTime(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	timeSet = true;
}

/**
 * @brief Gets current time from RTC.
 *
 * Wrapper for @ref HAL_RTC_GetTime function.
 */
void RTCController::getTime(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

bool RTCController::isTimeSet() const
{
	return timeSet;
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
	secCallbackReceivers.callAllReceivers(nullptr);

	// Minute overflow.
	if (rtc_time.Seconds == 0) {
		minuteCallbackReceivers.callAllReceivers(nullptr);
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
	minuteCallbackReceivers.insertBack(min_callback);
}

/**
 * @brief Registers an object for second callback.
 */
void RTCController::registerSecondCallback(ISecCallback* sec_callback)
{
	secCallbackReceivers.insertBack(sec_callback);
}

/**
 * @brief Unregisters an object from second callback.
 */
void RTCController::unregisterSecondCallback(ISecCallback* sec_callback)
{
	secCallbackReceivers.remove(sec_callback);
}

/**
 * @brief Unregisters an object from minute callback.
 */
void RTCController::unregisterMinuteCallback(IMinCallback *min_callback)
{
	minuteCallbackReceivers.remove(min_callback);
}
