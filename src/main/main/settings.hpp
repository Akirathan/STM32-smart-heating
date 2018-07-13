/**
 * @file settings.hpp
 * @author Pavel Marek
 * @date Mar 30, 2018
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#define CALLBACK_RECEIVERS_NUM_INPUT            1
#define CALLBACK_RECEIVERS_NUM_FRAME_TERMINATE  1
#define CALLBACK_RECEIVERS_NUM_EXIT_MSG         1
// Defines for RTCController
#define CALLBACK_RECEIVERS_NUM_SECOND_IT        1
#define CALLBACK_RECEIVERS_NUM_MINUTE_IT        4

#define WINDOW_SYSTEM_CTRL_WINDOWS              18
#define WINDOW_SYSTEM_STATIC_WINDOWS            5

#define INTERVALS_NUM                           15

// Maximum number of software timers
#define SW_TIMERS_NUM                           3

const char DEVICE_ID[]                          = "stm1";

#endif /* SETTINGS_HPP_ */
