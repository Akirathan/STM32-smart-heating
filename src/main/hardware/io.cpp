/**
 * @file input.cpp
 * @author Pavel Marek
 * @date 24.4.2017
 */

#include "io.hpp"
#include "rt_assert.h"
#include "input.hpp"

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

IO& IO::getInstance()
{
	static IO instance;
	return instance;
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


void IO::registerInputCallback(IInputCallback *inputCallback)
{
	callbackReceivers.insertBack(inputCallback);
}

void IO::task()
{
	if (!joystickInitialized) {
		uint8_t ret = BSP_JOY_Init(JOY_MODE_GPIO);
		rt_assert(ret == IO_OK, "Failed joystick initialization");

		joystickInitialized = true;
	}

	JOYState_TypeDef joy_state = BSP_JOY_GetState();

	if (joy_state == JOY_NONE) {
		return;
	}

	// Call all registered callback receivers.
	Input input;
	input.type = InputType::JOYSTICK;
	input.joyState = joy_state;
	callbackReceivers.callAllReceivers((void *) &input);
}

IO::IO() :
	joystickInitialized(false)
{
}
