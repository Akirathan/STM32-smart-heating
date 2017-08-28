/**
 * @file input.cpp
 * @author Pavel Marek
 * @date 24.4.2017
 */

#include "io.hpp"

namespace IO {

/**
 * @brief Waits for the user to press joystick in any direction.
 *
 * Joystick is considered pressed when it is pushed down and up again, so when
 * user holds the joystick down, nothing is happening.
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

} // namespace IO
