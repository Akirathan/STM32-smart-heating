/*
 * time_window.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#include "time_window.hpp"

using namespace std;

/**
 * Needed by intervalframe ctor
 */
time_window::time_window() :
		time_window{Coord{0, 0}}
{ }

/**
 * Initializes TimeWindow with 5 minute_jump.
 */
time_window::time_window(const Coord &coord) :
		control_window{coord}, selected_{NONE}, hours{0}, minutes{0}
{

}

/**
* Draw this window without any formatting ie. without any selected
* item.
*/
void time_window::nofont_draw() const {
	char text[5];
	sprintf(text, "%02lu:%02lu", this->hours, this->minutes);

	// Print the string
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *) text, LEFT_MODE);
}

/**
 * Draw the time window and the selected item inside it.
 * TODO draw error symbol
 */
void time_window::draw() const
{
	this->nofont_draw();

	char text[2];
	sFONT *font = BSP_LCD_GetFont();

	switch (this->selected_) {
	case NONE:
		/* Do nothing. Everything was done in noFontDraw */
		break;
	case MINUTES:
		/* Redraw just minutes */
		sprintf(text, "%02lu", this->minutes);

		this->save_font();

		// Print just minutes.
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width) * 3, this->coord_.y, (uint8_t *)text, LEFT_MODE);

		this->load_font();
		break;
	case HOURS:
		/* Redraw just hours */
		// Convert hours to string
		sprintf(text, "%02lu", this->hours);

		this->save_font();

		// Print just hours
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)text, LEFT_MODE);

		this->load_font();
		break;
	default:
		break;
	}
}

/**
 * Msg should be FOCUS_LEFT or FOCUS_RIGHT.
 */
void time_window::set_focus(Message msg) {
	switch (msg) {
	case Message::FOCUS_LEFT:
		this->set_selected(HOURS);
		break;
	case Message::FOCUS_RIGHT:
		this->set_selected(MINUTES);
		break;
	default:
		// Should not happen
		break;
	}

	this->draw(); // Redraw
}

Message time_window::event_handler(JOYState_TypeDef joy_state) {
	Message msg = Message::NONE;

	switch (joy_state) {
	case JOY_DOWN:
		if (this->selected_ == HOURS) {
			if (this->hours == 0) {
				this->hours = 23;
			}
			else {
				this->hours--;
			}
		}
		else if (this->selected_ == MINUTES) {
			if (this->minutes == 0) {
				this->minutes = 60 - this->minute_jump;
			}
			else {
				this->minutes -= this->minute_jump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_UP:
		if (this->selected_ == HOURS) {
			if (this->hours == 24) {
				this->hours = 0;
			}
			else {
				this->hours++;
			}
		}
		else if (this->selected_ == MINUTES) {
			if (this->minutes == 60 - this->minute_jump) {
				this->minutes = 0;
			}
			else {
				this->minutes += this->minute_jump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_RIGHT:
		if (this->selected_ == HOURS) {
			this->set_selected(MINUTES);
			msg = Message::NONE;
		}
		else if (this->selected_ == MINUTES) {
			this->set_selected(NONE);
			msg = Message::FOCUS_RIGHT;
		}
		else {
			// Minutes or hours should be selected, because
			// focus was set before call to this function.
			msg = Message::ERROR;
		}
		break;
	case JOY_LEFT:
		if (this->selected_ == HOURS) {
			this->set_selected(NONE);
			msg = Message::FOCUS_LEFT;
		}
		else if (this->selected_ == MINUTES) {
			this->set_selected(HOURS);
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

	this->draw(); // Redraw this window
	return msg;
}

/**
 * TODO: This method is no longer necessary.
 * @param sel should be NONE, HOURS or MINUTES.
 */
void time_window::set_selected(selected_t sel) {
	this->selected_ = sel;
}

void time_window::set_hours(uint32_t hrs)
{
	this->hours = hrs;
}

void time_window::set_minutes(uint32_t mins)
{
	this->minutes = mins;
}

uint32_t time_window::get_hours() const
{
	return this->hours;
}

uint32_t time_window::get_minutes() const
{
	return this->minutes;
}

