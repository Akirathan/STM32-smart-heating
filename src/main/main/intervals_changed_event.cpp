/**
 * @file intervals_changed_event.cpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#include "intervals_changed_event.hpp"
#include "rt_assert.h"


IntervalsChangedEvent::IntervalsChangedEvent(IntervalFrameData data[], const size_t count)
{
	rt_assert(count <= INTERVALS_NUM, "Too much intervals");

	for (size_t i = 0; i < count; i++) {
		this->data[i] = data[i];
	}
	this->count = count;
}

const IntervalFrameData * IntervalsChangedEvent::getData(size_t *count) const
{
	rt_assert(count != nullptr, "Unexpected null pointer");

	*count = this->count;
	return data;
}
