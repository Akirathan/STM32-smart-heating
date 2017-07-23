/*
 * rtc.cpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include "rtc.hpp"

#define ENABLE_SEC_INTERRUPT

/**
 * Second interrupt handler.
 */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	rtc& rtc = rtc::get_instance();
	rtc.update();
}

rtc& rtc::get_instance()
{
	static rtc instance;
	return instance;
}

/**
 * Initializes RTC peripheral. In case of wakeup from power reset mode,
 * RTC configuration is still saved in backup domain. This constructor
 * does not care about previous RTC initializations, and overwrites
 * this configuration into backup domain.
 */
rtc::rtc()
{
	RCC_OscInitTypeDef oscilator;

	/* Enable the LSE oscilator */
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

	/* Configure the clock source for RTC */
	RCC_PeriphCLKInitTypeDef periphClock;
	periphClock.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periphClock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClock) != HAL_OK) {
		// TODO: return HAL_ERROR;
	}

	__HAL_RCC_RTC_ENABLE(); // Enable RTC clock

	/* Initialize RTC, configure prescaler, etc. */
	this->handle.Instance = RTC;
	this->handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	HAL_RTC_Init(&this->handle);

#ifdef ENABLE_SEC_INTERRUPT
	// Configure the NVIC for RTC global interrupt
	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);

	// Enable the RTC global Interrupt
	HAL_NVIC_EnableIRQ(RTC_IRQn);

	// Enable second interrupt
	HAL_RTCEx_SetSecond_IT(&this->handle);
#else
	HAL_RTCEx_DeactivateSecond(&this->handle);
#endif
}

AppStatus_TypeDef rtc::set_time(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_SetTime(&this->handle, time, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		this->time_set = true;
		return APP_OK;
	}
}


AppStatus_TypeDef rtc::get_time(RTC_TimeTypeDef* time)
{
	if (HAL_RTC_GetTime(&this->handle, time, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		return APP_OK;
	}
}

bool rtc::is_time_set() const
{
	return this->time_set;
}

RTC_HandleTypeDef& rtc::get_handle()
{
	return this->handle;
}


/**
 * Called by second interrupt handler.
 */
void rtc::update()
{
	if (this->time_set == false) {
		return ;
	}

	RTC_TimeTypeDef rtc_time;
	this->get_time(&rtc_time);

	/* Second overflow */
	// Call all registered "functions"
	for (interface_sec_callback *sec_callback: this->second_callback_vec) {
		sec_callback->sec_callback();
	}

	/* Minute overflow */
	if (rtc_time.Seconds == 0) {
		// Call all registered "functions"
		for (interface_min_callback *min_callback : this->minute_callback_vec) {
			min_callback->min_callback();
		}
	}

	/* Hour overflow */
	if (rtc_time.Minutes == 0) {
		// ...
	}
}

void rtc::register_minute_callback(interface_min_callback *min_callback)
{
	this->minute_callback_vec.push_back(min_callback);
}

void rtc::register_second_callback(interface_sec_callback *sec_callback)
{
	this->second_callback_vec.push_back(sec_callback);
}

/**
 * Find pointer inside vector and deletes it.
 */
void rtc::unregister_second_callback(interface_sec_callback *sec_callback)
{
	for (auto it = this->second_callback_vec.begin(); it != this->second_callback_vec.end(); ++it) {
		if (*it == sec_callback) {
			this->second_callback_vec.erase(it);
			break;
		}
	}
}

/**
 * Find pointer inside vector and deletes it.
 */
void rtc::unregister_minute_callback(interface_min_callback *min_callback)
{
	for (auto it = this->minute_callback_vec.begin(); it != this->minute_callback_vec.end(); ++it) {
		if (*it == min_callback) {
			this->minute_callback_vec.erase(it);
			break;
		}
	}
}
