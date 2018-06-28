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
	inline ConnectedEvent(uint32_t curr_time, uint32_t server_real_time) :
		serverTime(server_real_time)
	{
		timeShift = server_real_time - curr_time;
	}

	/**
	 * Timeshift is number of seconds server is ahead from STM.
	 * @return
	 */
	inline uint32_t getTimeShift() const {
		return timeShift;
	}

	inline uint32_t getServerTime() const {
		return serverTime;
	}
private:
	uint32_t timeShift;
	uint32_t serverTime;
};



#endif /* CONNECTED_EVENT_HPP_ */
