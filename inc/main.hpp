#ifndef __MAIN_HPP
#define __MAIN_HPP

#include <memory> //
#include <stdlib.h>

/* STM includes. */
#include <one_wire.hpp>
#include <temp_sensor.hpp>
#include <temp_sensor.hpp>
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"
#include "stm3210c_eval_lcd.h"
#include "stm3210c_eval_sd.h"

/* My includes. */
#include "interval_frame.hpp"
#include "input.hpp"
//#include "relay.hpp"
//#include "rtc.hpp"

/* Public functions */
void second_it();

#endif //__MAIN_HPP
