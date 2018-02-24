/**
 * @file rtc_controller.hpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#ifndef RTC_CONTROLLER_HPP_
#define RTC_CONTROLLER_HPP_

#include <vector>
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "callbacks.hpp"

/**
 * @brief Class for controlling the RTC component.
 *
 * This class is basically a container for objects that are registered for
 * minute or second callbacks.
 *
 * @note Implemented as a singleton class.
 */
class RTCController {
public:
	static RTCController& getInstance();
	void operator=(const RTCController&) = delete;
	RTCController(const RTCController&) = delete;
	void setTime(RTC_TimeTypeDef* time);
	void getTime(RTC_TimeTypeDef* time);
	bool isTimeSet() const;
	void registerMinuteCallback(IMinCallback* min_callback);
	void registerSecondCallback(ISecCallback* sec_callback);
	void unregisterSecondCallback(ISecCallback* sec_callback);
	void unregisterMinuteCallback(IMinCallback* min_callback);
	void update();
private:
	RTCController();
	std::vector<IMinCallback *> minuteCallbackVec;
	std::vector<ISecCallback *> secondCallbackVec;
	bool timeSet = false;
};

#endif // RTC_CONTROLLER_HPP_
