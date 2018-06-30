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
#include "interval_list.hpp"

/**
 * Base class for @ref IntervalsChangedStmEvent and @ref IntervalsChangedServerEvent.
 *
 * This class contains methods that are common for both derived classes,
 * especially converting IntervalFrameData[] to IntervalList and vice
 * versa.
 *
 * @note This class cannot be instantiated.
 */
class IntervalsChangedEvent : public Event {
public:
	const IntervalFrameData * getData(size_t *count) const;
	uint32_t getTimestamp() const;
	bool isTimestampSynced() const;
protected:
	IntervalsChangedEvent(IntervalFrameData data[], const size_t count, const uint32_t timestamp,
	         	          bool timestamp_synced);
private:
	IntervalFrameData data[INTERVALS_NUM];
	size_t dataCount;
	IntervalList intervalList;
	uint32_t timestamp;
	bool timestampSynced;
};


#endif /* INTERVALS_CHANGED_EVENT_HPP_ */
