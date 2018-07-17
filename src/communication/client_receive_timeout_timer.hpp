/**
 * @file client_receive_timeout_timer.hpp
 * @author Pavel Marek
 * @date Jul 15, 2018
 */

#ifndef CLIENT_RECEIVE_TIMEOUT_TIMER_HPP_
#define CLIENT_RECEIVE_TIMEOUT_TIMER_HPP_

#include <cstdint>

/**
 * This timer represents "how long should we wait for an answer from server" timer.
 */
class ClientReceiveTimeoutTimer {
public:
	static void start();
	static void checkTimeout();
private:
	static const uint32_t TIMEOUT_MS = 15000;

	static bool started;
	static uint32_t startedTicks;
};


#endif /* CLIENT_RECEIVE_TIMEOUT_TIMER_HPP_ */
