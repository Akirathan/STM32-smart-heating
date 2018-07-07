/**
 * @file sw_timer_owner.hpp
 * @author Pavel Marek
 * @date Jul 7, 2018
 */

#ifndef SW_TIMER_OWNER_HPP_
#define SW_TIMER_OWNER_HPP_

#include <cstdint>

/**
 * Class inherits from this "abstract class" when it wants to receive SW timer
 * interrupts in certain timeout.
 *
 * This class does register itself for polling from @ref Application in constructor
 * and when timeout occurs, @ref timeout virtual method is called.
 *
 * Note that @ref start method must be called in order to receive any timeouts.
 */
class SwTimerOwner {
public:
	SwTimerOwner();
	virtual ~SwTimerOwner() = default;
	SwTimerOwner(unsigned int timeout);
	void poll();
	virtual void timeout() = 0;
protected:
	void startSwTimer();
	void stopSwTimer();
private:
	unsigned int timeoutMs;
	bool started;
	uint32_t startedTicks;
};



#endif /* SW_TIMER_OWNER_HPP_ */
