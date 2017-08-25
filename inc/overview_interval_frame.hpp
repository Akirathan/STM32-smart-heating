/*
 * overview_interval_frame.hpp
 *
 *  Created on: Aug 25, 2017
 *      Author: mayfa
 */

#ifndef OVERVIEW_INTERVAL_FRAME_HPP_
#define OVERVIEW_INTERVAL_FRAME_HPP_

#include <vector>
#include "interval_frame.hpp"
#include "interval_frame_data.hpp"
#include "static_time_window.hpp"
#include "static_temp_window.hpp"

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



#endif /* OVERVIEW_INTERVAL_FRAME_HPP_ */
