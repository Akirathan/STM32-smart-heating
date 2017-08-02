/*
 * window_system.cpp
 *
 *  Created on: Jul 13, 2017
 *      Author: mayfa
 */

#include "window_system.hpp"

using namespace std;

WindowSystem::WindowSystem()
	: windows(*this), currWindow(nullptr)
{ }

/**
 * Modules pass control to this method. Windows object then
 * controls all the user's input and pass control back to system
 * when some of the windows sends Message::EXIT message.
 */
AppStatus_TypeDef WindowSystem::passControl()
{
	/* Window managing */
	bool end = false;
	while (!end) {
		JOYState_TypeDef joy_state = read_joy();
		Message msg = currWindow->eventHandler(joy_state);
		switch (msg) {
		case Message::NONE:
			break;
		case Message::FOCUS_LEFT:
			windows.previous();
			break;
		case Message::FOCUS_RIGHT:
			windows.next();
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
WindowSystem::Windows::Windows(WindowSystem& system)
	: system(system), ctrlWindowIdx(0)
{ }

/**
 * Returns control window indexer.
 */
size_t WindowSystem::Windows::ctrlWindowIdxGet() const
{
	return ctrlWindowIdx;
}

/**
 * Increases control window indexer.
 */
void WindowSystem::Windows::ctrlWindowIdxInc()
{
	if (ctrlWindowIdx == ctrlWindows.size() - 1) {
		ctrlWindowIdx = 0;
	}
	else {
		ctrlWindowIdx++;
	}
}

/**
 * Decreases control window indexer.
 */
void WindowSystem::Windows::ctrlWindowIdxDec()
{
	if (ctrlWindowIdx == 0) {
		ctrlWindowIdx = ctrlWindows.size() - 1;
	}
	else {
		ctrlWindowIdx--;
	}
}

void WindowSystem::Windows::previous() {
	ctrlWindowIdxDec();
	system.currWindow = ctrlWindows[ctrlWindowIdxGet()];
	system.currWindow->setFocus(Message::FOCUS_RIGHT);
}

/**
 * Sets currWindow_ for System and focuses to the new currWindow_.
 */
void WindowSystem::Windows::next()
{
	ctrlWindowIdxInc();
	system.currWindow = ctrlWindows[ctrlWindowIdxGet()];
	system.currWindow->setFocus(Message::FOCUS_LEFT);
}

/**
 * Adds control window to the internal data representation and draw it.
 */
void WindowSystem::Windows::addControl(ControlWindow* window)
{
	ctrlWindows.push_back(window);

	// If this is the first added ControlWindow
	if (ctrlWindows.size() == 1) {
		/* Add it to ctrlWindows and set focus to it */
		system.currWindow = ctrlWindows[0];
		system.currWindow->setFocus(Message::FOCUS_LEFT);
	}

	// Draw this control window
	ctrlWindows[ctrlWindows.size()-1]->draw();
}

/**
 * Adds static window to the internal data representation and draw it.
 */
void WindowSystem::Windows::addStatic(StaticWindow* window)
{
	staticWindows.push_back(window);

	// Draw this static window
	staticWindows[staticWindows.size()-1]->draw();
}

void WindowSystem::addControl(ControlWindow* window)
{
	return windows.addControl(window);
}

void WindowSystem::addStatic(StaticWindow* window)
{
	return windows.addStatic(move(window));
}
