/**
 * @file timer_events.cpp
 * @author mayfa
 * @date Mar 30, 2018
 */

#include "timer_events.h"
#include "io.hpp"


void timer_event_rtc()
{

}

/**
 * Interface function between interrupt handler and application for user-input
 * task timer.
 */
void timer_event_user_input()
{
	IO::getInstance().task();
}

