/**
 * @file input.hpp
 * @author Pavel Marek
 * @date 24.4.2017
 * @brief This file includes simple functions for acquiring user's input.
 */

#ifndef INC_INPUT_HPP_
#define INC_INPUT_HPP_

#include <vector>
#include <unistd.h> //TODO: ?
#include "stm3210c_eval.h"
#include "callbacks.hpp"
#include "settings.h"

class IO {
public:
	static IO& getInstance();
	void operator=(const IO&) = delete;
	IO(const IO&) = delete;
	void print(char *ptr);
	void registerInputCallback(IInputCallback *inputCallback);
	void task();
private:
	IO();
	IInputCallback * callbackReceivers[CALLBACK_RECEIVERS_NUM_INPUT];
	size_t idx;
	bool joystickInitialized;
};

#endif /* INC_INPUT_HPP_ */
