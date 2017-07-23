/*
 * rtc.hpp
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 */

#ifndef RTC_HPP_
#define RTC_HPP_

#include <vector>
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "stats.hpp"
#include "callbacks.hpp"

/**
 * Singleton class.
 */
class rtc {
	rtc();
	RTC_HandleTypeDef handle;
	std::vector<interface_min_callback *> minute_callback_vec;
	std::vector<interface_sec_callback *> second_callback_vec;
	bool time_set = false;
public:
	static rtc & get_instance();
	void operator=(const rtc &) = delete;
	rtc(const rtc &) = delete;
	RTC_HandleTypeDef & get_handle();
	AppStatus_TypeDef set_time(RTC_TimeTypeDef *time);
	AppStatus_TypeDef get_time(RTC_TimeTypeDef *time);
	bool is_time_set() const;
	void register_minute_callback(interface_min_callback *min_callback);
	void register_second_callback(interface_sec_callback *sec_callback);
	void unregister_second_callback(interface_sec_callback *sec_callback);
	void unregister_minute_callback(interface_min_callback *min_callback);
	void update();
};

#endif // RTC_HPP_
