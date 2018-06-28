/**
 * @file connected_event.hpp
 * @author Pavel Marek
 * @date Jun 28, 2018
 */

#include <cstdint>
#include "event.hpp"

#ifndef CONNECTED_EVENT_HPP_
#define CONNECTED_EVENT_HPP_

/**
 * @brief Object representing "connected to the server" event.
 */
class ConnectedEvent : public Event {
public:
	inline ConnectedEvent(uint32_t time_shift) : timeShift(time_shift)
	{}

	inline uint32_t getTimeShift() const {
		return timeShift;
	}
private:
	uint32_t timeShift;
};



#endif /* CONNECTED_EVENT_HPP_ */
