/**
 * @file key_set_event.hpp
 * @author Pavel Marek
 * @date Jul 8, 2018
 */

#ifndef KEY_SET_EVENT_HPP_
#define KEY_SET_EVENT_HPP_

#include "event.hpp"
#include "des_key.hpp"

/**
 * Represents "DES key was set on STM" event.
 */
class KeySetEvent: public Event {
public:
	inline KeySetEvent(const DesKey &key) :
		key(key)
	{ }

	inline const DesKey getKey() const
	{
		return key;
	}

private:
	DesKey key;
};

#endif /* KEY_SET_EVENT_HPP_ */
