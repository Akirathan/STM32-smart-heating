/**
 * @file key_frame.cpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#include "key_frame.hpp"
#include "lcd.hpp"

KeyFrame::KeyFrame()
{
	uint32_t x_size = LCD::get_x_size();
	uint32_t first_row_y = LINE(3);
	uint16_t x_0 = x_size/8 - 5;
	uint16_t x_1 = 2 * (x_size/8) - 5;
	// ...
	uint16_t submit_button_x = x_size/2 - 20;
	uint16_t submit_button_y = LINE(6);

	hexCharWindows[0] = HexCharWindow(Coord(x_0, first_row_y));
	hexCharWindows[1] = HexCharWindow(Coord(x_1, first_row_y));

	submitButton = Button(Coord(submit_button_x, submit_button_y), "SUBMIT");

	windowSystem.addControl(&hexCharWindows[0]);
	windowSystem.addControl(&hexCharWindows[1]);
	windowSystem.addControl(&submitButton);
}

void KeyFrame::passControl()
{
	windowSystem.registerExitMessageCallbackReceiver(this);
	windowSystem.run();
}

void KeyFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();
	callTerminateCallbackReceivers();
}

void KeyFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

void KeyFrame::drawHeader()
{
	const uint8_t *header = reinterpret_cast<const uint8_t *>("Insert key from server");
	LCD::draw_header(const_cast<uint8_t *>(header));
}

