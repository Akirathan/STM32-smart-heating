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

class Application : public IFrameTerminateCallback {
public:
	Application();
	static void setCurrFrame(IFrame* frame);
	virtual void frameTerminateCallback() override;
	virtual void registerFrameTerminateCallback() override;
	void switchCurrFrameToMain();
	void run();
private:
	static IFrame* currFrame;
	static bool clearDisplayFlag;
	ClkFrame clkFrame;
	MainFrame mainFrame;
	SetIntervalFrame setIntervalFrame;
};



#endif /* APPLICATION_HPP_ */
