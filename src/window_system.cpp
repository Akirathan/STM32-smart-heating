/*
 * window_system.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: mayfa
 */

#include "window_system.hpp"

using namespace std;

window_system::window_system() : windows_{*this}, curr_window{nullptr}
{

}

/**
 * Modules pass control to this method. Windows object then
 * controls all the user's input and pass control back to system
 * when some of the windows sends Message::EXIT message.
 */
AppStatus_TypeDef window_system::pass_control()
{
	/* Window managing */
	bool end = false;
	while (!end) {
		JOYState_TypeDef joy_state = read_joy();
		Message msg = curr_window->event_handler(joy_state);
		switch (msg) {
		case Message::NONE:
			break;
		case Message::FOCUS_LEFT:
			windows_.previous();
			break;
		case Message::FOCUS_RIGHT:
			windows_.next();
			break;
		case Message::ERROR:
			// TODO ...
			break;
		case Message::EXIT:
			end = true;
		}
	}

	/* Data gathering */

	return APP_OK;
}

/*** System::Windows ***/
window_system::windows::windows(window_system &system) : system{system}, ctrl_window_idx{0}
{

}

/**
 * Returns control window indexer.
 */
size_t window_system::windows::ctrl_window_idx_get() const
{
	return ctrl_window_idx;
}

/**
 * Increases control window indexer.
 */
void window_system::windows::ctrl_window_idx_inc()
{
	if (ctrl_window_idx == ctrl_windows.size() - 1) {
		ctrl_window_idx = 0;
	}
	else {
		ctrl_window_idx++;
	}
}

/**
 * Decreases control window indexer.
 */
void window_system::windows::ctrl_window_idx_dec()
{
	if (ctrl_window_idx == 0) {
		ctrl_window_idx = ctrl_windows.size() - 1;
	}
	else {
		ctrl_window_idx--;
	}
}

void window_system::windows::previous() {
	ctrl_window_idx_dec();
	system.curr_window = ctrl_windows[ctrl_window_idx_get()];
	system.curr_window->set_focus(Message::FOCUS_RIGHT);
}

/**
 * Sets curr_window_ for System and focuses to the new curr_window_.
 */
void window_system::windows::next()
{
	ctrl_window_idx_inc();
	system.curr_window = ctrl_windows[ctrl_window_idx_get()];
	system.curr_window->set_focus(Message::FOCUS_LEFT);
}

/**
 * Adds control window to the internal data representation and draw it.
 */
void window_system::windows::add_control(control_window *window)
{
	ctrl_windows.push_back(window);

	// If this is the first added ControlWindow
	if (ctrl_windows.size() == 1) {
		/* Add it to ctrl_windows and set focus to it */
		system.curr_window = ctrl_windows[0];
		system.curr_window->set_focus(Message::FOCUS_LEFT);
	}

	// Draw this control window
	ctrl_windows[ctrl_windows.size()-1]->draw();
}

/**
 * Adds static window to the internal data representation and draw it.
 */
void window_system::windows::add_static(static_window *window)
{
	static_windows.push_back(window);

	// Draw this static window
	static_windows[static_windows.size()-1]->draw();
}

void window_system::add_control(control_window *window)
{
	return this->windows_.add_control(window);
}

void window_system::add_static(static_window *window)
{
	return this->windows_.add_static(move(window));
}
