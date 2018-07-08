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
		// Call all exit message callback receivers.
		callbackReceivers.callAllReceivers(nullptr);
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
	callbackReceivers.insertBack(exitMessageCallback);
}

void WindowSystem::unregisterExitMessageCallbackReceiver(IExitMessageCallback *exitMessageCallback)
{
	callbackReceivers.remove(exitMessageCallback);
}

/**
 * Frames pass control to this method.
 */
void WindowSystem::run()
{
	registerInputCallback();
	windows.resetFocus();
}

void WindowSystem::stop()
{
	IO::getInstance().unregisterInputCallback(this);
}

WindowSystem::Windows::Windows(WindowSystem& system)
	: system(system),
	  ctrlWindowIdx(0),
	  ctrlWindowsCount(0),
	  staticWindowsCount(0),
	  ctrlWindows{nullptr},
	  staticWindows{nullptr},
	  toBeClearedWindows{nullptr},
	  toBeClearedWindowsIdx(0)
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
	if (ctrlWindowIdx == ctrlWindowsCount - 1) {
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
		ctrlWindowIdx = ctrlWindowsCount - 1;
	}
	else {
		ctrlWindowIdx--;
	}
}

bool WindowSystem::Windows::contains(Window *window)
{
	for (size_t i = 0; i < ctrlWindowsCount; i++) {
		if (ctrlWindows[i] == window) {
			return true;
		}
	}

	for (size_t i = 0; i < staticWindowsCount; i++) {
		if (staticWindows[i] == window) {
			return true;
		}
	}
	return false;
}

/**
 * Adds given window to the list of windows that should be clear when GUI task
 * will run.
 */
void WindowSystem::Windows::addToToBeClearedWindows(Window *window)
{
	rt_assert(toBeClearedWindowsIdx < WINDOW_SYSTEM_CTRL_WINDOWS, "Too much windows in toBeClearedWindow list");
	toBeClearedWindows[toBeClearedWindowsIdx] = window;
	toBeClearedWindowsIdx++;
}

void WindowSystem::Windows::clearPendingWindows()
{
	for (size_t i = 0; i < toBeClearedWindowsIdx; i++) {
		toBeClearedWindows[i]->clear();
		toBeClearedWindows[i] = nullptr;
	}
	toBeClearedWindowsIdx = 0;
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
	rt_assert(ctrlWindowsCount < WINDOW_SYSTEM_CTRL_WINDOWS,
			"Attempting to add too much control windows");

	rt_assert(!contains(window), "Window is already contained in this system");

	ctrlWindows[ctrlWindowsCount] = window;
	ctrlWindowsCount++;

	// If this is the first added ControlWindow.
	if (ctrlWindowsCount == 1) {
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
	rt_assert(staticWindowsCount < WINDOW_SYSTEM_STATIC_WINDOWS,
			"Attempting to add too much static windows");

	rt_assert(!contains(window), "Window is already contained in this system");

	staticWindows[staticWindowsCount] = window;
	staticWindowsCount++;
}

void WindowSystem::Windows::removeControl(IControlWindow* window)
{
	int to_be_removed_idx = -1;
	for (size_t i = 0; i < ctrlWindowsCount; i++) {
		if (ctrlWindows[i] == window) {
			to_be_removed_idx = static_cast<int>(i);
			break;
		}
	}

	rt_assert(to_be_removed_idx != -1, "Control window must be in the system before removal");
	addToToBeClearedWindows(ctrlWindows[to_be_removed_idx]);

	// Shift rest of the ctrlWindows
	ctrlWindowsCount--;
	for (size_t i = to_be_removed_idx; i < ctrlWindowsCount; i++) {
		ctrlWindows[i] = ctrlWindows[i+1];
	}
	ctrlWindows[ctrlWindowsCount] = nullptr;

	if (system.currWindow == window) {
		previous();
	}
}

/**
 * @brief Redraw all windows.
 */
void WindowSystem::Windows::drawAllWindows()
{
	clearPendingWindows();

	for (size_t i = 0; i < staticWindowsCount; ++i) {
		staticWindows[i]->redraw();
	}

	for (size_t i = 0; i < ctrlWindowsCount; ++i) {
		ctrlWindows[i]->redraw();
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
 * @attention One window cannot be added more than once.
 * @attention Order of adding is important, see docs for class.
 */
void WindowSystem::addControl(IControlWindow* window)
{
	window->setRedrawFlag();
	windows.addControl(window);
}

/**
 * @brief Adds one static window.
 * @attention One window cannot be added more than once.
 */
void WindowSystem::addStatic(IStaticWindow* window)
{
	window->setRedrawFlag();
	windows.addStatic(window);
}

/**
 * Removes given control window. @p window must be in this WindowSystem before.
 * If the window to be removed is currently focues, the previous control window
 * is focused.
 * @param window ... Window to be removed.
 */
void WindowSystem::removeControl(IControlWindow* window)
{
	windows.removeControl(window);
}

/**
 * Disposes all windows.
 */
void WindowSystem::clear()
{
	// TODO
}

void WindowSystem::drawAllWindows()
{
	windows.drawAllWindows();
}

void WindowSystem::setForRedraw()
{
	windows.setAllForRedraw();
}
