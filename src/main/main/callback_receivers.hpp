/**
 * @file callback_receivers.hpp
 * @author mayfa
 * @date Mar 31, 2018
 */

#ifndef CALLBACK_RECEIVERS_HPP_
#define CALLBACK_RECEIVERS_HPP_

#include "rt_assert.h"

/**
 * Helper class for any type that is capable of registering other types for callbacks.
 * receiver_t must be subclass of ICallbackReceiver (must contain callback method).
 */
template <size_t size, typename receiver_t>
class CallbackReceivers {
public:
	CallbackReceivers();
	void insertBack(receiver_t *receiver);
	void remove(receiver_t *receiver);
	void callAllReceivers(void *param);

private:
	receiver_t * receivers[size];
	size_t count;
};

template <size_t size, typename receiver_t>
inline CallbackReceivers<size, receiver_t>::CallbackReceivers()
	: count(0)
{
	for (size_t i = 0; i < size; ++i) {
		receivers[i] = nullptr;
	}
}

template <size_t size, typename receiver_t>
inline void CallbackReceivers<size, receiver_t>::insertBack(receiver_t* receiver)
{
	rt_assert(count < size, "Too much receivers");

	receivers[count] = receiver;
	count++;
}

template <size_t size, typename receiver_t>
inline void CallbackReceivers<size, receiver_t>::remove(receiver_t* receiver)
{
	size_t i = 0;
	while (receivers[i] != receiver && i <= size) {
		i++;
	}

	if (i >= size) {
		// Attempting to remove non-existing callback receiver.
		return;
	}

	receivers[i] = nullptr;

	// Shift rest of receivers array.
	for (size_t rest_i = i; rest_i < size - 1; ++rest_i) {
		receivers[rest_i] = receivers[rest_i + 1];
		receivers[rest_i + 1] = nullptr;
	}

	count--;
}

/**
 * Calls all registered callback-receivers.
 */
template <size_t size, typename receiver_t>
inline void CallbackReceivers<size, receiver_t>::callAllReceivers(void *param)
{
	for (size_t i = 0; i < size; ++i) {
		if (receivers[i] == nullptr) {
			break;
		}
		receivers[i]->callback(param);
	}
}

#endif /* CALLBACK_RECEIVERS_HPP_ */
