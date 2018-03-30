/**
 * @file timer_events.h
 * @author mayfa
 * @date Mar 30, 2018
 *
 * @brief Interface file between cube-mx (interrupts) and application.
 */

#ifndef TIMER_EVENTS_H_
#define TIMER_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

void timer_event_rtc();
void timer_event_user_input();

#ifdef __cplusplus
}
#endif

#endif /* TIMER_EVENTS_H_ */
