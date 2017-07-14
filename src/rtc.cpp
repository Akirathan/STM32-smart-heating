/*
 * rtc.c
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#include "rtc.hpp"

/* Private variables */
RTC_HandleTypeDef rtc_handle;

/**
 * Second interrupt. Just call handler from the main module.
 */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc) {
	second_it();
}

/**
 * Initialize the RTC peripheral with LSE clock source.
 */
HAL_StatusTypeDef rtc_Init() {
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
		return HAL_ERROR;
	}

	/* Configure the clock source for RTC */
	RCC_PeriphCLKInitTypeDef periphClock;
	periphClock.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periphClock.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClock) != HAL_OK) {
		return HAL_ERROR;
	}

	__HAL_RCC_RTC_ENABLE(); // Enable RTC clock

	/* Initialize RTC, configure prescaler, etc. */
	rtc_handle.Instance = RTC;
	rtc_handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	HAL_RTC_Init(&rtc_handle);

	/* Enable second interrupt */
	HAL_RTCEx_SetSecond_IT(&rtc_handle);

	return HAL_OK;
}

AppStatus_TypeDef rtc_SetTime(RTC_TimeTypeDef *rtcTime) {
	if (HAL_RTC_SetTime(&rtc_handle, rtcTime, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		return APP_OK;
	}
}

AppStatus_TypeDef rtc_GetTime(RTC_TimeTypeDef *rtcTime) {
	if (HAL_RTC_GetTime(&rtc_handle, rtcTime, RTC_FORMAT_BIN) != HAL_OK) {
		return APP_ERROR;
	}
	else {
		return APP_OK;
	}
}

