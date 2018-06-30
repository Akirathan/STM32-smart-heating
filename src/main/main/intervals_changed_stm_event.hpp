/**
 * @file intervals_changed_stm_event.hpp
 * @author mayfa
 * @date Jun 30, 2018
 */

#ifndef INTERVALS_CHANGED_STM_EVENT_HPP_
#define INTERVALS_CHANGED_STM_EVENT_HPP_

#include "intervals_changed_event.hpp"

/**
 * @brief Represents "intervals changed in STM" event.
 *
 * This event is generated when user sets new intervals into the device ie.
 * after @ref SetIntervalFrame terminates.
 *
 * This class contains also timestamp in which this event was generated and
 * bool flag whether the time was synchronized with the server when this
 * event was generated.
 *
 * @note All data members are immutable.
 *
 * See @ref Application::emitEvent for more details.
 */
class IntervalsChangedStmEvent : public IntervalsChangedEvent {
public:
	inline IntervalsChangedStmEvent(IntervalFrameData data[], const size_t count, const uint32_t timestamp,
	         	                    bool timestamp_synced)
		: IntervalsChangedEvent(data, count, timestamp, timestamp_synced)
	{ }
};




#endif /* INTERVALS_CHANGED_STM_EVENT_HPP_ */
