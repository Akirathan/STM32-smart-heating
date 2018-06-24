/**
 * @file intervals_changed_event.hpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#ifndef INTERVALS_CHANGED_EVENT_HPP_
#define INTERVALS_CHANGED_EVENT_HPP_

#include <cstddef> // For size_t
#include "event.hpp"
#include "interval_frame_data.hpp"
#include "settings.h" // For INTERVALS_NUM

/**
 * Represents intervals changed event.
 * Event arguments are immutable.
 *
 * See @ref Application::emitEvent for more details.
 */
class IntervalsChangedEvent : public Event {
public:
	IntervalsChangedEvent(IntervalFrameData data[], const size_t count);
	const IntervalFrameData * getData(size_t *count) const;
private:
	IntervalFrameData data[INTERVALS_NUM];
	size_t count;
};


#endif /* INTERVALS_CHANGED_EVENT_HPP_ */
