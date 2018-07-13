/**
 * @file input.hpp
 * @author Pavel Marek
 * @date 24.4.2017
 * @brief This file includes simple functions for acquiring user's input.
 */

#ifndef INC_INPUT_HPP_
#define INC_INPUT_HPP_

#include <settings.hpp>
#include <unistd.h> //TODO: ?
#include "stm3210c_eval.h"
#include "callbacks.hpp"
#include "callback_receivers.hpp"

class IO {
public:
	static IO& getInstance();
	void operator=(const IO&) = delete;
	IO(const IO&) = delete;
	void print(char *ptr);
	void registerInputCallback(IInputCallback *inputCallback);
	void unregisterInputCallback(IInputCallback *inputCallback);
	void task();
private:
	IO();
	CallbackReceivers<CALLBACK_RECEIVERS_NUM_INPUT, IInputCallback> callbackReceivers;
	bool joystickInitialized;
};

#endif /* INC_INPUT_HPP_ */
