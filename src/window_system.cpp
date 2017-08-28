/**
 * @file window_system.cpp
 * @author Pavel Marek
 * @date 13.7.2017
 */

#include "window_system.hpp"

WindowSystem::WindowSystem()
	: windows(*this), currWindow(nullptr)
{ }

/**
 * Frames pass control to this method. Windows object then
 * controls all the user's input and pass control back to system
 * when some of the windows sends Message::EXIT message.
 */
AppStatus_TypeDef WindowSystem::passControl()
{
	// Window managing.
	bool end = false;
	while (!end) {
		JOYState_TypeDef joy_state = IO::read_joy();
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

	return APP_OK;
}

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

/**
 * Sets focus to previous control window.
 */
void WindowSystem::Windows::previous() {
	ctrlWindowIdxDec();
	system.currWindow = ctrlWindows[ctrlWindowIdxGet()];
	system.currWindow->setFocus(Message::FOCUS_RIGHT);
}

/**
 * Sets focus to next control window.
 */
void WindowSystem::Windows::next()
{
	ctrlWindowIdxInc();
	system.currWindow = ctrlWindows[ctrlWindowIdxGet()];
	system.currWindow->setFocus(Message::FOCUS_LEFT);
}

/**
 * Adds control window to the internal data representation and draws it.
 */
void WindowSystem::Windows::addControl(IControlWindow* window)
{
	ctrlWindows.push_back(window);

	// If this is the first added ControlWindow.
	if (ctrlWindows.size() == 1) {
		// Add it to ctrlWindows and set focus to it.
		system.currWindow = ctrlWindows[0];
		system.currWindow->setFocus(Message::FOCUS_LEFT);
	}

	// Draw this control window.
	ctrlWindows[ctrlWindows.size()-1]->draw();
}

/**
 * Adds static window to the internal data representation and draws it.
 */
void WindowSystem::Windows::addStatic(IStaticWindow* window)
{
	staticWindows.push_back(window);

	// Draw this static window.
	staticWindows[staticWindows.size()-1]->draw();
}

void WindowSystem::addControl(IControlWindow* window)
{
	return windows.addControl(window);
}

void WindowSystem::addStatic(IStaticWindow* window)
{
	return windows.addStatic(window);
}
