/**
 * @file time_window.cpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#include "time_window.hpp"

TimeWindow::TimeWindow(const Coord& coord)
	: IControlWindow(coord)
{ }

/**
* Draws this window without any formatting ie. without any selected item.
*/
void TimeWindow::noFontDraw() const
{
	char text[5];
	sprintf(text, "%02lu:%02lu", hours, minutes);

	// Print the string
	BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *) text, LEFT_MODE);
}

/**
 * @brief Draws the time window and the selected item inside it.
 */
void TimeWindow::draw() const
{
	noFontDraw();

	char text[2];
	sFONT *font = BSP_LCD_GetFont();

	switch (selected) {
	// Do nothing. Everything was done in noFontDraw.
	case NONE:
		break;

	// Redraw just minutes.
	case MINUTES:
		sprintf(text, "%02lu", minutes);

		saveFont();

		// Print just minutes.
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(coord.x + (font->Width) * 3, coord.y, (uint8_t *)text, LEFT_MODE);

		loadFont();
		break;

	// Redraw just hours.
	case HOURS:
		// Convert hours to string.
		sprintf(text, "%02lu", hours);

		saveFont();

		// Print just hours.
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(coord.x, coord.y, (uint8_t *)text, LEFT_MODE);

		loadFont();
		break;
	default:
		break;
	}
}

void TimeWindow::setFocus(Message msg)
{
	switch (msg) {
	case Message::FOCUS_LEFT:
		setSelected(HOURS);
		break;
	case Message::FOCUS_RIGHT:
		setSelected(MINUTES);
		break;
	default:
		// Should not happen.
		break;
	}

	draw();
}

Message TimeWindow::eventHandler(JOYState_TypeDef joy_state)
{
	Message msg = Message::NONE;

	switch (joy_state) {
	case JOY_DOWN:
		if (selected == HOURS) {
			if (hours == 0) {
				hours = 23;
			}
			else {
				hours--;
			}
		}
		else if (selected == MINUTES) {
			if (minutes == 0) {
				minutes = 60 - minuteJump;
			}
			else {
				minutes -= minuteJump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_UP:
		if (selected == HOURS) {
			if (hours == 24) {
				hours = 0;
			}
			else {
				hours++;
			}
		}
		else if (selected == MINUTES) {
			if (minutes == 60 - minuteJump) {
				minutes = 0;
			}
			else {
				minutes += minuteJump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_RIGHT:
		if (selected == HOURS) {
			setSelected(MINUTES);
			msg = Message::NONE;
		}
		else if (selected == MINUTES) {
			setSelected(NONE);
			msg = Message::FOCUS_RIGHT;
		}
		else {
			// Minutes or hours should be selected, because
			// focus was set before call to this function.
			msg = Message::ERROR;
		}
		break;
	case JOY_LEFT:
		if (selected == HOURS) {
			setSelected(NONE);
			msg = Message::FOCUS_LEFT;
		}
		else if (selected == MINUTES) {
			setSelected(HOURS);
			msg = Message::NONE;
		}
		else {
			// Minutes or hours should be selected, because
			// focus was set before call to this function.
			// TODO draw error symbol
			msg = Message::ERROR;
		}
		break;
	default:
		// Prevent redrawing
		return Message::NONE;
	}

	draw(); // Redraw this window
	return msg;
}

/**
 * TODO: This method is no longer necessary.
 * @param sel should be NONE, HOURS or MINUTES.
 */
void TimeWindow::setSelected(selected_t sel)
{
	selected = sel;
}

void TimeWindow::setHours(uint32_t hrs)
{
	hours = hrs;
}

void TimeWindow::setMinutes(uint32_t mins)
{
	minutes = mins;
}

uint32_t TimeWindow::getHours() const
{
	return hours;
}

uint32_t TimeWindow::getMinutes() const
{
	return minutes;
}

