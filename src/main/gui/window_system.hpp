/**
 * @file window_system.hpp
 * @author Pavel Marek
 * @date 13.7.2017
 */

#ifndef INC_WINDOW_SYSTEM_HPP_
#define INC_WINDOW_SYSTEM_HPP_

#include <vector>
#include "io.hpp"
#include "control_window.hpp"
#include "callbacks.hpp"
#include "callback_receivers.hpp"
#include "static_window.hpp"
#include "input.hpp"

/**
 * @brief Window controller for frames.
 *
 * This class is used for frames to control their windows. Windows are passed
 * via @ref addControl and @ref addStatic methods. @ref passControl passes
 * control to WindowSystem until the @ref Message::EXIT is returned from one
 * of the windows. WindowSystem manages focus of all the control windows and
 * calls their @ref IControlWindow::eventHandler method accordingly.
 *
 * Windows are ordered as they are added - user should ensure to add all the
 * windows with respect to their location, otherwise focus-passing between
 * control windows would be in "random" order (specifically in order in which
 * the windows were added).
 *
 * First focused window is the first control window inserted via @ref addControl
 *
 * @note Note that the order is especially important for control windows.
 */
class WindowSystem : public IInputCallback {
private:
	class Windows {
	public:
		Windows(WindowSystem &system);
		void previous();
		void next();
		void addControl(IControlWindow* window);
		void addStatic(IStaticWindow* window);
		void drawAllWindows();
		void setAllForRedraw();
		void resetFocus();
	private:
		friend class WindowSystem;
		WindowSystem& system;
		size_t ctrlWindowIdx;
		std::vector<IControlWindow *> ctrlWindows;
		std::vector<IStaticWindow *> staticWindows;
		size_t ctrlWindowIdxGet() const;
		void ctrlWindowIdxInc();
		void ctrlWindowIdxDec();
	};
public:
	virtual void inputCallback(Input input) override;
	virtual void registerInputCallback() override;
	void registerExitMessageCallbackReceiver(IExitMessageCallback *exitMessageCallback);
	void unregisterExitMessageCallbackReceiver(IExitMessageCallback *exitMessageCallback);
	void run();
	void stop();
	void addControl(IControlWindow* window);
	void addStatic(IStaticWindow* window);
	void clear();
	void drawAllWindows();
	void setForRedraw();
	WindowSystem();
private:
	friend class Windows;
	Windows windows;
	IControlWindow* currWindow;
	bool joyCallbackRegistered;
	CallbackReceivers<CALLBACK_RECEIVERS_NUM_EXIT_MSG, IExitMessageCallback> callbackReceivers;
};


#endif /* INC_WINDOW_SYSTEM_HPP_ */
