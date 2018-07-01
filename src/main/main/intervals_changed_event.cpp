/**
 * @file intervals_changed_event.cpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#include "intervals_changed_event.hpp"
#include "rt_assert.h"

const IntervalFrameData * IntervalsChangedEvent::getData(size_t *count) const
{
	rt_assert(count != nullptr, "Unexpected null pointer");

	*count = dataCount;
	return data;
}

const IntervalList & IntervalsChangedEvent::getList() const
{
	return intervalList;
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

IntervalsChangedEvent::IntervalsChangedEvent(IntervalFrameData data[], const size_t count, const uint32_t timestamp,
		bool timestamp_synced) :
	dataCount(count),
	timestamp(timestamp),
	timestampSynced(timestamp_synced)
{
	rt_assert(count <= INTERVALS_NUM, "Too much intervals");

	// Initialize data.
	for (size_t i = 0; i < count; i++) {
		this->data[i] = data[i];
	}

	// Initialize intervalList.
	convertDataToList(&intervalList, data, dataCount);
	intervalList.setTimestamp(timestamp);
}

IntervalsChangedEvent::IntervalsChangedEvent(const IntervalList &interval_list, bool timestamp_synced) :
	dataCount(interval_list.getIntervalsCount()),
	intervalList(interval_list),
	timestamp(interval_list.getTimestamp()),
	timestampSynced(timestamp_synced)
{
	rt_assert(interval_list.getIntervalsCount() <= INTERVALS_NUM, "Too much intervals");
	rt_assert(interval_list.getTimestamp() != 0, "Timestamp must be set");

	// Initialize data.
	for (size_t i = 0; i < interval_list.getIntervalsCount(); i++) {
		data[i].from = interval_list.getInterval(i)->getFromTime();
		data[i].to = interval_list.getInterval(i)->getToTime();
		data[i].temp = interval_list.getInterval(i)->getTemp();
	}
}

void IntervalsChangedEvent::convertDataToList(IntervalList *interval_list,
		const IntervalFrameData data[], size_t data_count)
{
	rt_assert(interval_list != nullptr, "Unexpected nullptr");

	for (size_t i = 0; i < data_count; i++) {
		Interval interval(data[i].from, data[i].to, data[i].temp);
		interval_list->addInterval(interval);
	}
}
