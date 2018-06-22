/**
 * @file frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

#include "window_system.hpp"
#include "callbacks.hpp"
#include "callback_receivers.hpp"

/**
 * @brief Base class for every frame in the application.
 */
class IFrame {
public:
	inline IFrame() : headerRedrawed(false) {}

	virtual ~IFrame() = default;

	/**
	 * This method is called when control switches to this frame ie. this frame
	 * is going to be drawed in GUI task.
	 * It is called from @ref Application::setCurrFrame
	 *
	 * This method should reset state of frame - it is supposed that control may
	 * switch to one frame more than once.
	 * This means that this method should behave like constructor that can be
	 * called more than once.
	 */
	virtual void passControl() =0;

	/**
	 * WindowSystem is supposed to contain all windows (ie. disposable data)
	 * so one definition for all frames here is OK.
	 */
	inline void clear()
	{
		windowSystem.clear();
	}

	/**
	 * Redraw all windows that windowSystem contains.
	 * Also calls @ref drawHeader if this is the first redraw call on this
	 * frame.
	 */
	inline void redraw()
	{
		if (!headerRedrawed) {
			drawHeader();
			headerRedrawed = true;
		}

		windowSystem.drawAllWindows();
	}

	void setForRedraw();

	inline void registerFrameTerminateCallbackReceiver(IFrameTerminateCallback *receiver)
	{
		callbackReceivers.insertBack(receiver);
	}

	inline void unregisterFrameTerminateCallbackReceiver(IFrameTerminateCallback *receiver)
	{
		callbackReceivers.remove(receiver);
	}

protected:
	WindowSystem windowSystem;

	inline void callTerminateCallbackReceivers()
	{
		callbackReceivers.callAllReceivers(nullptr);
	}

private:
	CallbackReceivers<CALLBACK_RECEIVERS_NUM_FRAME_TERMINATE, IFrameTerminateCallback> callbackReceivers;
	bool headerRedrawed;

	virtual void drawHeader() = 0;
};

#endif /* FRAME_HPP_ */
