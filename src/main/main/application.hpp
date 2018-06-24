/**
 * @file application.hpp
 * @author mayfa
 * @date Mar 30, 2018
 */

#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "frame.hpp"
#include "clk_frame.hpp"
#include "main_frame.hpp"
#include "set_interval_frame.hpp"
#include "callbacks.hpp"
#include "measured_temp_event.hpp"
#include "intervals_changed_event.hpp"

/**
 * This class contains main application logic in its @ref run method.
 * This method contains indefinite loop.
 *
 * Serves as authority for dispatching global event through emitEvent methods.
 * These methods should be called by whoever generated the event.
 */
class Application : public IFrameTerminateCallback {
public:
	Application();
	static void setCurrFrame(IFrame* frame);
	virtual void frameTerminateCallback() override;
	virtual void registerFrameTerminateCallback() override;
	void switchCurrFrameToMain();
	void run();
	static void emitEvent(const MeasuredTempEvent &event);
	static void emitEvent(const IntervalsChangedEvent &event);
private:
	static IFrame* currFrame;
	static bool clearDisplayFlag;
	ClkFrame clkFrame;
	MainFrame mainFrame;
	SetIntervalFrame setIntervalFrame;

	void guiTask();
};



#endif /* APPLICATION_HPP_ */
