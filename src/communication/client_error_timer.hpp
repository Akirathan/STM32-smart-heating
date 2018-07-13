/**
 * @file client_error_timer.hpp
 * @author Pavel Marek
 * @date Jul 13, 2018
 */

#ifndef CLIENT_ERROR_TIMER_HPP_
#define CLIENT_ERROR_TIMER_HPP_

#include <cstdint>

/**
 * When an network-related error occurs during the "communication cycle", this
 * cycle should be restarted in some timeout.
 *
 * This class represents a timer that is started when the above mentioned error
 * occurs and restarts the communication cycle when period of this timer elapses.
 */
class ClientErrorTimer {
public:
	static void start();
	static void checkTimeout();
private:
	static const uint32_t TIMEOUT_MS = 5000;

	static bool started;
	static uint32_t startedTicks;
};


#endif /* CLIENT_ERROR_TIMER_HPP_ */
