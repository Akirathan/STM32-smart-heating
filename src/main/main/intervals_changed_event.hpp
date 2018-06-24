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
 * Represents "intervals changed" event.
 * This class contains also timestamp in which this event was generated and
 * bool flag whether the time was synchronized with the server when this
 * event was generated.
 *
 * @note All data members are immutable.
 *
 * See @ref Application::emitEvent for more details.
 */
class IntervalsChangedEvent : public Event {
public:
	IntervalsChangedEvent(IntervalFrameData data[], const size_t count, const uint32_t timestamp,
	         	          bool timestamp_synced);
	const IntervalFrameData * getData(size_t *count) const;
	uint32_t getTimestamp() const;
	bool isTimestampSynced() const;
private:
	IntervalFrameData data[INTERVALS_NUM];
	size_t count;
	uint32_t timestamp;
	bool timestampSynced;
};


#endif /* INTERVALS_CHANGED_EVENT_HPP_ */
