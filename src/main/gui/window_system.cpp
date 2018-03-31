/**
 * @file window_system.cpp
 * @author Pavel Marek
 * @date 13.7.2017
 */

#include "window_system.hpp"

WindowSystem::WindowSystem()
	: windows(*this),
	  currWindow(nullptr),
	  joyCallbackRegistered(false)
{ }

void WindowSystem::inputCallback(Input input)
{
	if (input.type != InputType::JOYSTICK) {
		return;
	}

	Message msg = currWindow->eventHandler(input.joyState);
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
		// Call all registered callback receivers.
		for (IExitMessageCallback *receiver : exitMsgCallbackReceivers) {
		  receiver->exitMessageCallback();
		}
		break;
	}
}

void WindowSystem::registerInputCallback()
{
	IO::getInstance().registerInputCallback(this);
}

/**
 * Registers receiver object of Exit Message events.
 * @param exitMessageCallback Receiver object
 */
void WindowSystem::registerExitMessageCallbackReceiver(IExitMessageCallback *exitMessageCallback)
{
	exitMsgCallbackReceivers.push_back(exitMessageCallback);
}

void WindowSystem::unregisterExitMessageCallbackReceiver(IExitMessageCallback *exitMessageCallback)
{
	for (auto it = exitMsgCallbackReceivers.begin(); it != exitMsgCallbackReceivers.end(); ++it) {
		if (*it == exitMessageCallback) {
			exitMsgCallbackReceivers.erase(it);
			return;
		}
	}

	// TODO: rt_assert(false, "No such receiver");
}

/**
 * Frames pass control to this method.
 */
void WindowSystem::run()
{
	registerInputCallback();
	windows.resetFocus();
}

WindowSystem::Windows::Windows(WindowSystem& system)
	: system(system),
	  ctrlWindowIdx(0)
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
 * Adds control window to the internal data representation.
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

}

/**
 * Adds static window to the internal data representation.
 */
void WindowSystem::Windows::addStatic(IStaticWindow* window)
{
	staticWindows.push_back(window);
}


/**
 * @note Should be called just once.
 */
void WindowSystem::Windows::drawAllWindows()
{
	for (IStaticWindow *static_window : staticWindows) {
		static_window->redraw();
	}

	for (IControlWindow *ctrl_window : ctrlWindows) {
		ctrl_window->redraw();
	}
}

void WindowSystem::Windows::setAllForRedraw()
{
	for (size_t i = 0; i < staticWindowsCount; ++i) {
		staticWindows[i]->setRedrawFlag();
	}

	for (size_t i = 0; i < ctrlWindowsCount; ++i) {
		ctrlWindows[i]->setRedrawFlag();
	}
}

/**
 * Resets focus to the first control window.
 */
void WindowSystem::Windows::resetFocus()
{
	system.currWindow = ctrlWindows[0];
	system.currWindow->setFocus(Message::FOCUS_LEFT);
}

/**
 * @brief Adds one control window.
 * @attention Order of adding is important, see docs for class.
 */
void WindowSystem::addControl(IControlWindow* window)
{
	return windows.addControl(window);
}

void WindowSystem::addStatic(IStaticWindow* window)
{
	return windows.addStatic(window);
}

/**
 * Disposes all windows.
 */
void WindowSystem::clear()
{

}

void WindowSystem::drawAllWindows()
{
	windows.drawAllWindows();
}

void WindowSystem::setForRedraw()
{
	windows.setAllForRedraw();
}
