/*
 * window_system.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: mayfa
 */

#include "window_system.hpp"

using namespace std;

WindowSystem::WindowSystem() : windows_{*this}, curr_window_{nullptr}
{

}

AppStatus_TypeDef WindowSystem::pass_control()
{
	/* Window managing */
	bool end = false;
	while (!end) {
		JOYState_TypeDef joy_state = read_joy();
		Message msg = curr_window_->event_handler(joy_state);
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
WindowSystem::Windows::Windows(WindowSystem &system) : system{system}, ctrl_window_idx_{0}
{

}

/**
 * Returns control window indexer.
 */
size_t WindowSystem::Windows::ctrl_window_idx_get() const
{
	return ctrl_window_idx_;
}

/**
 * Increases control window indexer.
 */
void WindowSystem::Windows::ctrl_window_idx_inc()
{
	if (ctrl_window_idx_ == ctrlWindows_.size() - 1) {
		ctrl_window_idx_ = 0;
	}
	else {
		ctrl_window_idx_++;
	}
}

/**
 * Decreases control window indexer.
 */
void WindowSystem::Windows::ctrl_window_idx_dec()
{
	if (ctrl_window_idx_ == 0) {
		ctrl_window_idx_ = ctrlWindows_.size() - 1;
	}
	else {
		ctrl_window_idx_--;
	}
}

void WindowSystem::Windows::previous() {
	ctrl_window_idx_dec();
	system.curr_window_ = ctrlWindows_[ctrl_window_idx_get()].get();
	system.curr_window_->setFocus(Message::FOCUS_RIGHT);
}

/**
 * Sets curr_window_ for System and focuses to the new curr_window_.
 */
void WindowSystem::Windows::next()
{
	ctrl_window_idx_inc();
	system.curr_window_ = ctrlWindows_[ctrl_window_idx_get()].get();
	system.curr_window_->setFocus(Message::FOCUS_LEFT);
}

/**
 * Adds control window to the internal data representation and draw it.
 */
window_id WindowSystem::Windows::add_control(unique_ptr<ControlWindow> window)
{
	ctrlWindows_.push_back(move(window));

	// If this is the first added ControlWindow
	if (ctrlWindows_.size() == 1) {
		/* Add it to ctrlWindows_ and set focus to it */
		system.curr_window_ = static_cast<ControlWindow *>(ctrlWindows_[0].get());
		system.curr_window_->setFocus(Message::FOCUS_LEFT);
	}

	// Draw this control window
	ctrlWindows_[ctrlWindows_.size()-1]->draw();
}

/**
 * Adds static window to the internal data representation and draw it.
 */
window_id WindowSystem::Windows::add_static(unique_ptr<StaticWindow> window)
{
	staticWindows_.push_back(move(window));

	// Draw this static window
	staticWindows_[staticWindows_.size()-1]->draw();
}

window_id WindowSystem::add_control(std::unique_ptr<ControlWindow> window)
{
	this->windows_.add_control(move(window));
	return this->windows_.ctrlWindows_.size() - 1; // Last index to vector
}

window_id WindowSystem::add_static(std::unique_ptr<StaticWindow> window)
{
	this->windows_.add_static(move(window));
}

ControlWindow& WindowSystem::get_control(window_id ctrl_id) const
{
	//return this->windows_.ctrlWindows_.at(ctrl_id);
}
