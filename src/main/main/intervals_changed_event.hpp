/**
 * @file intervals_changed_event.hpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#ifndef INTERVALS_CHANGED_EVENT_HPP_
#define INTERVALS_CHANGED_EVENT_HPP_

#include "event.hpp"
#include "interval_frame_data.hpp"

/**
 * Represents intervals changed event.
 *
 * See @ref Application::emitEvent for more details.
 */
class IntervalsChangedEvent : public Event {
public:
	IntervalsChangedEvent(IntervalFrameData data[], size_t count);
};


#endif /* INTERVALS_CHANGED_EVENT_HPP_ */
