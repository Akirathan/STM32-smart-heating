/*
 * interval_window.cpp
 *
 *  Created on: 18. 2. 2017
 *      Author: Mayfa
 */

#include "interval_window.hpp"

using namespace std;

/**
 * Initializes TimeWindow with 5 minute_jump.
 */
TimeWindow::TimeWindow(const Coord &coord) :
		TimeWindow{coord, 5}
{

}

/**
 * @param minute_jump should be multiple of 5.
 */
TimeWindow::TimeWindow(const Coord &coord, uint32_t minute_jump) :
		ControlWindow{coord}, selected_{NONE}, hours_{0}, minutes_{0}, minute_jump{minute_jump}
{

}

/**
* Draw this window without any formatting ie. without any selected
* item.
*/
void TimeWindow::noFontDraw() const {
	char text[5];
	sprintf(text, "%02lu:%02lu", this->hours_, this->minutes_);

	// Print the string
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *) text, LEFT_MODE);
}

/**
 * Draw the time window and the selected item inside it.
 * TODO draw error symbol
 */
void TimeWindow::draw() const
{
	this->noFontDraw();

	char text[2];
	sFONT *font = BSP_LCD_GetFont();

	switch (this->selected_) {
	case NONE:
		/* Do nothing. Everything was done in noFontDraw */
		break;
	case MINUTES:
		/* Redraw just minutes */
		sprintf(text, "%02lu", this->minutes_);

		this->saveFont();

		// Print just minutes.
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(this->coord_.x + (font->Width) * 3, this->coord_.y, (uint8_t *)text, LEFT_MODE);

		this->loadFont();
		break;
	case HOURS:
		/* Redraw just hours */
		// Convert hours to string
		sprintf(text, "%02lu", this->hours_);

		this->saveFont();

		// Print just hours
		BSP_LCD_SetTextColor(SEL_COLOR);
		BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)text, LEFT_MODE);

		this->loadFont();
		break;
	default:
		break;
	}
}

/**
 * Msg should be FOCUS_LEFT or FOCUS_RIGHT.
 */
void TimeWindow::setFocus(Message msg) {
	switch (msg) {
	case Message::FOCUS_LEFT:
		this->setSelected(HOURS);
		break;
	case Message::FOCUS_RIGHT:
		this->setSelected(MINUTES);
		break;
	default:
		// Should not happen
		break;
	}

	this->draw(); // Redraw
}

Message TimeWindow::event_handler(JOYState_TypeDef joy_state) {
	Message msg = Message::NONE;

	switch (joy_state) {
	case JOY_DOWN:
		if (this->selected_ == HOURS) {
			if (this->hours_ == 0) {
				this->hours_ = 23;
			}
			else {
				this->hours_--;
			}
		}
		else if (this->selected_ == MINUTES) {
			if (this->minutes_ == 0) {
				this->minutes_ = 60 - this->minute_jump;
			}
			else {
				this->minutes_ -= this->minute_jump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_UP:
		if (this->selected_ == HOURS) {
			if (this->hours_ == 24) {
				this->hours_ = 0;
			}
			else {
				this->hours_++;
			}
		}
		else if (this->selected_ == MINUTES) {
			if (this->minutes_ == 60 - this->minute_jump) {
				this->minutes_ = 0;
			}
			else {
				this->minutes_ += this->minute_jump;
			}
		}
		msg = Message::NONE;
		break;
	case JOY_RIGHT:
		if (this->selected_ == HOURS) {
			this->setSelected(MINUTES);
			msg = Message::NONE;
		}
		else if (this->selected_ == MINUTES) {
			this->setSelected(NONE);
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
			this->setSelected(NONE);
			msg = Message::FOCUS_LEFT;
		}
		else if (this->selected_ == MINUTES) {
			this->setSelected(HOURS);
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
void TimeWindow::setSelected(selected_t sel) {
	this->selected_ = sel;
}

TempWindow::TempWindow(const Coord &coord) :
		ControlWindow{coord}, temp_{this->low_bond}, focused_{false}
{

}

Message TempWindow::event_handler(JOYState_TypeDef joy_state) {
	switch (joy_state) {
	case JOY_DOWN:
		if (this->temp_ == this->low_bond) {
			this->temp_ = this->high_bond;
		}
		else {
			this->temp_--;
		}
		this->draw(); // Redraw
		break;
	case JOY_UP:
		if (this->temp_ == this->high_bond) {
			this->temp_ = this->low_bond;
		}
		else {
			this->temp_++;
		}
		this->draw(); // Redraw
		break;
	case JOY_LEFT:
		this->unsetFocus();
		return Message::FOCUS_LEFT;
	case JOY_RIGHT:
		this->unsetFocus();
		return Message::FOCUS_RIGHT;
	default:
		return Message::NONE;
	}
	return Message::NONE;
}

/**
 * Suppose font is already set.
 */
void TempWindow::draw()const
{
	BSP_LCD_DisplayStringAt(this->coord_.x, this->coord_.y, (uint8_t *)this->getTemp().c_str(), LEFT_MODE);
}

/**
 * Redraws this window and loads font.
 */
void TempWindow::unsetFocus()
{
	this->focused_ = false;
	this->loadFont();
	this->draw();
}

/**
 * Saves current font for further drawing and
 * immediately redraws this window.
 */
void TempWindow::setFocus(Message msg) {
	this->focused_ = true;
	this->saveFont();
	BSP_LCD_SetTextColor(SEL_COLOR);
	this->draw();
}

/**
 * Convert temperature member temp_ to string.
 */
string TempWindow::getTemp() const {
	char buff[2];
	sprintf(buff, "%02lu", this->temp_);

	string s{buff};
	return s;
}
