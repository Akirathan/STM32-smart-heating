/*
 * rtc.h
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#ifndef RTC_H_
#define RTC_H_

#include "main.hpp" //TODO: ??
#include "stats.hpp"

/* Public functions */
HAL_StatusTypeDef rtc_Init();
AppStatus_TypeDef rtc_SetTime(RTC_TimeTypeDef *rtcTime);

#endif // RTC_H_
