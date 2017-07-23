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

void print(char *ptr)
{
	// Find out size
	char *ptr_cnt = ptr;
	int size = 0;
	while (*ptr_cnt++ != '\0') {
		++size;
	}

	write(0, ptr, size);
}
