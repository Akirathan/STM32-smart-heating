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
 * Date is set to 1.1.1970.
 */
void RTCController::setTime(const Time::Time &time)
{
	HAL_StatusTypeDef status = RTC_WriteTimeCounter(&hrtc, time.hours * 3600 + time.minutes * 60 + time.seconds);
	rt_assert(status == HAL_OK, "RTC cannot set counter");

	timeSet = true;
}

/**
 * @brief Gets current time from RTC.
 */
Time::Time RTCController::getTime()
{
	uint32_t time_counter = RTC_ReadTimeCounter(&hrtc);

	uint32_t hours = time_counter / 3600;
	uint32_t minutes = time_counter / 60;
	uint32_t hours_today = hours % 24;
	uint32_t minutes_today = (minutes % (24 * 60)) - (hours_today * 60);
	uint32_t seconds_today = time_counter % 60;

	return Time::Time(hours_today, minutes_today, seconds_today);
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

	Time::Time time = getTime();

	// Second overflow. Call all registered "functions".
	secCallbackReceivers.callAllReceivers(nullptr);

	// Minute overflow.
	if (time.seconds == 0) {
		minuteCallbackReceivers.callAllReceivers(nullptr);
	}

	// Hour overflow
	if (time.minutes == 0) {
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
