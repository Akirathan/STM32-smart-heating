/**
 * @file input.cpp
 * @author Pavel Marek
 * @date 24.4.2017
 */

#include "io.hpp"

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

void IO::print(char *ptr)
{
	// Find out size
	char *ptr_cnt = ptr;
	int size = 0;
	while (*ptr_cnt++ != '\0') {
		++size;
	}

	write(0, ptr, size);
}

void IO::registerJoyCallback(IJoystickCallback *joyCallback)
{

}


void IO::registerInputCallback(IInputCallback *inputCallback)
{

}
