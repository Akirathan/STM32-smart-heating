/**
 * @file measured_temp_event.hpp
 * @author mayfa
 * @date Jun 24, 2018
 */

#ifndef MEASURED_TEMP_EVENT_HPP_
#define MEASURED_TEMP_EVENT_HPP_

#include "event.hpp"
#include <cstdint>

/**
 * Represents measured temperature event and its arguments.
 * See @ref Application::emitEvent for more details.
 */
class MeasuredTempEvent : public Event {
public:
	double temp;
};



#endif /* MEASURED_TEMP_EVENT_HPP_ */
