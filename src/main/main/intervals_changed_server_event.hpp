/**
 * @file intervals_changed_server_event.hpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#ifndef INTERVALS_CHANGED_SERVER_EVENT_HPP_
#define INTERVALS_CHANGED_SERVER_EVENT_HPP_

#include "intervals_changed_event.hpp"

/**
 * @brief Represents "intervals changed on server" event.
 *
 * This event is generated in @ref CommunicationDevice::receiveIntervalsCb,
 * when this device downloads intervals from the server.
 *
 * @see Application::emitEvent
 */
class IntervalsChangedServerEvent : public IntervalsChangedEvent {
public:
	inline IntervalsChangedServerEvent(const IntervalList &interval_list, bool timestamp_synced) :
		IntervalsChangedEvent(interval_list, timestamp_synced)
	{ }
};


#endif /* INTERVALS_CHANGED_SERVER_EVENT_HPP_ */
