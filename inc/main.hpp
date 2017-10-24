/**
 * @file main.hpp
 * @author Pavel Marek
 */

#ifndef __MAIN_HPP
#define __MAIN_HPP

// STM includes.
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "stm3210c_eval_lcd.h"
#include "stm3210c_eval_sd.h"
#include "stm3210c_eval_eeprom.h"

// lwIP includes.
#include "ethernetif.h"
#include "init.h"
#include "tcp.h"

// My includes.
#include "interval_frame.hpp"
#include "main_frame.hpp"
#include "io.hpp"
#include "clk_frame.hpp"
#include "rtc_controller.hpp"
#include "eeprom.hpp"
#include "one_wire.hpp"
#include "temp_sensor.hpp"
#include "static_time_window.hpp" //

// Test includes
#include "temp_sensor_tests.hpp"
#include "relay.hpp" //

#endif //__MAIN_HPP
