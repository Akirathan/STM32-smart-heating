/**
 * @file key_frame.cpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#include "key_frame.hpp"
#include "lcd.hpp"

KeyFrame::KeyFrame()
{
	sFONT *font = LCD::get_font();
	uint32_t x_size = LCD::get_x_size();

	// Count all coordinates.
	uint32_t first_row_y = LINE(4);
	uint32_t second_row_y = LINE(6);

	uint16_t x[HEX_CHAR_WINDOWS_SIZE/2];
	x[0] = x_size/8 - 5;
	for (size_t i = 1; i < HEX_CHAR_WINDOWS_SIZE/2; i++) {
		x[i] = x[i-1] + font->Width * 2;
	}

	uint16_t submit_button_x = x_size/2 - 30;
	uint16_t submit_button_y = LINE(8);

	// Create all window objects.
	for (size_t i = 0; i < HEX_CHAR_WINDOWS_SIZE/2; i++) {
		hexCharWindows[i] = HexCharWindow(Coord(x[i], first_row_y));
	}
	for (size_t i = HEX_CHAR_WINDOWS_SIZE/2; i < HEX_CHAR_WINDOWS_SIZE; i++) {
		hexCharWindows[i] = HexCharWindow(Coord(x[i % (HEX_CHAR_WINDOWS_SIZE/2)], second_row_y));
	}
	submitButton = Button(Coord(submit_button_x, submit_button_y), "SUBMIT");

	// Add all window objects to windowSystem.
	for (size_t i = 0; i < HEX_CHAR_WINDOWS_SIZE; i++) {
		windowSystem.addControl(&hexCharWindows[i]);
	}
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
	const uint8_t *header = reinterpret_cast<const uint8_t *>("Insert key");
	LCD::draw_header(const_cast<uint8_t *>(header));
}

