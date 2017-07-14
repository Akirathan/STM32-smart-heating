/*
 * clk_window.h
 *
 *  Created on: 4.2.2017
 *      Author: Mayfa
 *
 * This file contains function prototypes for user time selection.
 * The user time selection consists of three windows - hours, minutes
 * and ok (button). User can switch between those windows with
 * left and right joystick buttons. When hours or minutes are selected
 * the up and down joystick buttons are used for increase/decrease
 * the values inside those windows.
 *
 * Use: Call the clk_window_Show function as first and then
 * clk_window_ReadUserInput.
 */

#ifndef CLK_WINDOW_H_
#define CLK_WINDOW_H_

#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "stm3210c_eval_io.h"
#include "stm3210c_eval_lcd.h"
#include "main.hpp"
#include "stats.hpp"

typedef enum {
	HOURS,
	MINUTES,
	OK
} clk_window_window_t;

AppStatus_TypeDef clk_window_Show();
AppStatus_TypeDef clk_window_ReadUserInput(RTC_TimeTypeDef *RTC_Time);

#endif /* WINDOW_H_ */
