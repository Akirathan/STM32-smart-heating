//
// Created by mayfa on 14.5.18.
//

#include "interval_list.hpp"
#include "rt_assert.h"

IntervalList IntervalList::deserialize(const uint8_t *buffer, const size_t buff_size)
{
    rt_assert(buff_size % Interval::SIZE == 0, "IntervalList: wrong size of buffer");

    IntervalList interval_list;
    for (size_t buff_idx = 0; buff_idx < buff_size; buff_idx += Interval::SIZE) {
        Interval interval = Interval::deserialize(buffer + buff_idx, Interval::SIZE);
        interval_list.addInterval(interval);
    }

    return interval_list;
}

IntervalList::IntervalList() :
        intervalsIdx(0),
        timeStamp(0)
{}

void IntervalList::setTimestamp(uint32_t time_stamp)
{
    timeStamp = time_stamp;
}

uint32_t IntervalList::getTimestamp() const
{
    return timeStamp;
}

void IntervalList::addInterval(const Interval &interval)
{
    rt_assert(intervalsIdx <= MAX_NUM, "IntervalList: too much intervals");
    intervals[intervalsIdx++] = interval;
}

/**
 * Returns interval at specified index.
 * @param index
 * @return may return nullptr if specified index is out of range
 */
const Interval * IntervalList::getInterval(size_t index) const
{
    if (index >= intervalsIdx) {
        return nullptr;
    }
    return intervals + index;
}

size_t IntervalList::getIntervalsCount() const
{
    return intervalsIdx;
}

bool IntervalList::isEmpty() const
{
	return intervalsIdx == 0;
}

/**
 * Serializes this IntervalList into given buffer.
 *
 * @param buffer   ... buffer to serialize this IntervalList in.
 * @param buff_len ... if not nullptr, length of buffer will be filled.
 */
void IntervalList::serialize(uint8_t *buffer, size_t *buff_len) const
{
    size_t buff_idx = 0;
    for (size_t i = 0; i < intervalsIdx; ++i) {
        intervals[i].serialize(buffer + buff_idx);
        buff_idx += Interval::SIZE;
    }

    if (buff_len != nullptr) {
        *buff_len = intervalsIdx * Interval::SIZE;
    }
}
