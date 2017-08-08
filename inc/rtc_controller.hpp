/*
 * rtc.hpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#ifndef RTC_CONTROLLER_HPP_
#define RTC_CONTROLLER_HPP_

#include <vector>
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "stats.hpp"
#include "callbacks.hpp"

/**
 * Singleton class.
 */
class RTCController {
public:
	static RTCController& getInstance();
	void operator=(const RTCController&) = delete;
	RTCController(const RTCController&) = delete;
	RTC_HandleTypeDef& getHandle();
	AppStatus_TypeDef setTime(RTC_TimeTypeDef* time);
	AppStatus_TypeDef getTime(RTC_TimeTypeDef* time);
	bool isTimeSet() const;
	void registerMinuteCallback(IMinCallback* min_callback);
	void registerSecondCallback(ISecCallback* sec_callback);
	void unregisterSecondCallback(ISecCallback* sec_callback);
	void unregisterMinuteCallback(IMinCallback* min_callback);
	void update();
private:
	RTCController();
	RTC_HandleTypeDef handle;
	std::vector<IMinCallback *> minuteCallbackVec;
	std::vector<ISecCallback *> secondCallbackVec;
	bool timeSet = false;
};

#endif // RTC_CONTROLLER_HPP_
