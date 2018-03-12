/**
 * @file input.cpp
 * @author Pavel Marek
 * @date 24.4.2017
 */

#include "io.hpp"

static bool initialized = false;

extern "C" {
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (BSP_IO_ITGetStatus(JOY_SEL_PIN) != 0) {

	}
	else if (BSP_IO_ITGetStatus(JOY_LEFT_PIN) != 0) {

	}
	else if (BSP_IO_ITGetStatus(JOY_UP_PIN) != 0) {

	}
	else if (BSP_IO_ITGetStatus(JOY_RIGHT_PIN) != 0) {

	}
	else if (BSP_IO_ITGetStatus(JOY_DOWN_PIN) != 0) {

	}

	// Clear IO expander IT
	BSP_IO_ITClear(JOY_ALL_PINS);
}
}

namespace IO {

/**
 * @brief Waits for the user to press joystick in any direction.
 *
 * Joystick is considered pressed when it is pushed down and up again, so when
 * user holds the joystick down, nothing is happening.
 */
JOYState_TypeDef read_joy()
{
	if (!initialized) {
		if (BSP_JOY_Init(JOY_MODE_GPIO) != HAL_OK) {
			Error_Handler();
		}
		initialized = true;
	}

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
