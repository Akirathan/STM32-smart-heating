/**
 * @file settings.h
 * @author mayfa
 * @date Mar 30, 2018
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define CALLBACK_RECEIVERS_NUM_INPUT            1
#define CALLBACK_RECEIVERS_NUM_FRAME_TERMINATE  1
#define CALLBACK_RECEIVERS_NUM_EXIT_MSG         1
// Defines for RTCController
#define CALLBACK_RECEIVERS_NUM_SECOND_IT        1
#define CALLBACK_RECEIVERS_NUM_MINUTE_IT        4

#define WINDOW_SYSTEM_CTRL_WINDOWS              5
#define WINDOW_SYSTEM_STATIC_WINDOWS            5

#define INTERVALS_NUM                           2


#define IP_ADDR0                                192
#define IP_ADDR1                                168
#define IP_ADDR2                                0
#define IP_ADDR3                                2

#define NETMASK_ADDR0                           255
#define NETMASK_ADDR1                           255
#define NETMASK_ADDR2                           255
#define NETMASK_ADDR3                           0

#define GW_ADDR0                                192
#define GW_ADDR1                                168
#define GW_ADDR2                                0
#define GW_ADDR3                                1

#endif /* SETTINGS_H_ */
