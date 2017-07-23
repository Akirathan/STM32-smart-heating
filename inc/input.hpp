/*
 * input.hpp
 *
 *  Created on: 24. 4. 2017
 *      Author: Mayfa
 *
 * This file includes simple functions for acquiring user's input.
 */

#ifndef INC_INPUT_HPP_
#define INC_INPUT_HPP_

#include <unistd.h>
#include "stm3210c_eval.h"

JOYState_TypeDef read_joy();
void print(char *ptr);


#endif /* INC_INPUT_HPP_ */
