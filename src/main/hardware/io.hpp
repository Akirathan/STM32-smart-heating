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

class IO {
public:
	static void print(char *ptr);
	static void registerInputCallback(IInputCallback *inputCallback);
	static void task();
private:
	static IInputCallback callbackReceivers[CALLBACK_RECEIVERS_NUM_INPUT];
	static size_t idx;
};

#endif /* INC_INPUT_HPP_ */
