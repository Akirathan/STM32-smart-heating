/**
 * @file intervals_changed_event.cpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#include "intervals_changed_event.hpp"
#include "rt_assert.h"


IntervalsChangedEvent::IntervalsChangedEvent(IntervalFrameData data[], const size_t count, const uint32_t timestamp,
		bool timestamp_synced) :
	count(count),
	timestamp(timestamp),
	timestampSynced(timestamp_synced)
{
	rt_assert(count <= INTERVALS_NUM, "Too much intervals");

	for (size_t i = 0; i < count; i++) {
		this->data[i] = data[i];
	}
}

const IntervalFrameData * IntervalsChangedEvent::getData(size_t *count) const
{
	rt_assert(count != nullptr, "Unexpected null pointer");

	*count = this->count;
	return data;
}

uint32_t IntervalsChangedEvent::getTimestamp() const
{
	return timestamp;
}

/**
 * Returns bool whether time was synchronized when this event was generated.
 * @return
 */
bool IntervalsChangedEvent::isTimestampSynced() const
{
	return timestampSynced;
}
