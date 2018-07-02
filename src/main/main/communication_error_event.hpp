/**
 * @file communation_error_event.hpp
 * @author mayfa
 * @date Jul 2, 2018
 */

#ifndef COMMUNICATION_ERROR_EVENT_HPP_
#define COMMUNICATION_ERROR_EVENT_HPP_

#include "event.hpp"

/**
 * @brief Represents "communication between STM and server error" event.
 *
 * See @ref Application::emitEvent for more information about events.
 */
class CommunicationErrorEvent : public Event {
public:
	enum type_t {
		UNKNOWN,
		ETH_LINK_DOWN,
		TCP_ERROR,
		HTTP_ERROR
	};

	inline CommunicationErrorEvent(type_t type) :
		type(type)
	{ }

private:
	type_t type;
};


#endif /* COMMUNICATION_ERROR_EVENT_HPP_ */
