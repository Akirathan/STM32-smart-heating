/**
 * @file input.hpp
 * @author mayfa
 * @date Mar 29, 2018
 */

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include "stm3210c_eval.h"

enum class InputType {
	NONE,
	JOYSTICK
};

struct Input {
	InputType type;
	JOYState_TypeDef joyState;

	inline Input() :
		type(InputType::NONE),
		joyState(JOY_NONE)
	{}
};



#endif /* INPUT_HPP_ */
