/**
 * @file client_timer.hpp
 * @author Pavel Marek
 * @date Jun 30, 2018
 */

#ifndef CLIENT_TIMER_HPP_
#define CLIENT_TIMER_HPP_

#include <cstdint>

/**
 * @brief A timer for @ref Client.
 *
 * This timer is started at the end of the communication cycle in @ref Client.
 * After this timer expires, @ref Client::startCycle is called.
 *
 * @ref checkTimeout must be called periodically.
 *
 * Note that it is unnecessary to initialize dedicated HW timer.
 */
class ClientTimer {
public:
	static void start();
	static void checkTimeout();
private:
	static const uint32_t TIMEOUT_MS = 3000;

	static bool started;
	static uint32_t startedTicks;
};



#endif /* CLIENT_TIMER_HPP_ */
