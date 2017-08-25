/*
 * interval_frame.hpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#ifndef INC_INTERVALFRAME_HPP_
#define INC_INTERVALFRAME_HPP_

#include <button.hpp>
#include <interval_frame_data.hpp>
#include <vector>
#include "stm3210c_eval.h"
#include "input.hpp"
#include "stats.hpp"
#include "time.hpp"
#include "time_window.hpp"
#include "static_time_window.hpp"
#include "temp_window.hpp"
#include "static_temp_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"
#include "frame.hpp"

class IntervalFrame : public IFrame {
public:
	IntervalFrame();
	virtual ~IntervalFrame() = default;
protected:
	Button nextButton;
	Button endButton;
	void drawHeader();
};

/**
 * Used for setting intervals.
 */
class SetIntervalFrame : public IntervalFrame {
public:
	SetIntervalFrame();
	std::vector<IntervalFrameData>& getData();
	virtual void passControl() override;
private:
	TimeWindow timeFromWindow;
	TimeWindow timeToWindow;
	TempWindow tempWindow;
	std::vector<IntervalFrameData> data;
	IntervalFrameData processInterval();
	void drawHeader();
};

/**
 * Used for overview of intervals. When user presses overview
 * button in main frame.
 */
class OverviewIntervalFrame : public IntervalFrame {
public:
	OverviewIntervalFrame(const std::vector<IntervalFrameData>& data);
	virtual void passControl() override;
private:
	StaticTimeWindow timeFromWindow;
	StaticTimeWindow timeToWindow;
	StaticTempWindow tempWindow;
	const std::vector<IntervalFrameData>& data;
	void printData(const IntervalFrameData& data);
	void drawHeader();
};

#endif /* INC_INTERVALFRAME_HPP_ */
