/**
 * @file timer_events.cpp
 * @author mayfa
 * @date Mar 30, 2018
 */

#include "timer_events.h"
#include "io.hpp"
#include "eeprom.hpp" // For EEPROM::isOperationInProgress


void timer_event_rtc()
{

}

/**
 * Interface function between interrupt handler and application for user-input
 * task timer.
 *
 * User input task is done only if EEPROM is not in read/write operation - this
 * is because joystick and EEPROM are using same I2C bus.
 */
void timer_event_user_input()
{
	if (!EEPROM::getInstance().isOperationInProgress()) {
		IO::getInstance().task();
	}
}

