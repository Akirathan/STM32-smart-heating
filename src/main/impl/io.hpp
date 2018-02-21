/**
 * @file input.hpp
 * @author Pavel Marek
 * @date 24.4.2017
 * @brief This file includes simple functions for acquiring user's input.
 */

#ifndef INC_INPUT_HPP_
#define INC_INPUT_HPP_

#include <unistd.h>
#include "stm3210c_eval.h"

namespace IO {

JOYState_TypeDef read_joy();
void print(char *ptr);

} // namespace IO

#endif /* INC_INPUT_HPP_ */
