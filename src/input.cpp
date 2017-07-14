/*
 * input.cpp
 *
 *  Created on: 24. 4. 2017
 *      Author: Mayfa
 */

#include "input.hpp"

/**
 * Wait for the user to push joystick in any direction.
 */
JOYState_TypeDef read_joy()
{
	BSP_JOY_Init(JOY_MODE_GPIO);

	while (1) {
		JOYState_TypeDef joyState = BSP_JOY_GetState();

		if (joyState != JOY_NONE) {
			while (BSP_JOY_GetState() == joyState);
			return joyState;
		}
	}
}
