/*
 * set_interval_frame.hpp
 *
 *  Created on: Aug 25, 2017
 *      Author: mayfa
 */

#ifndef SET_INTERVAL_FRAME_HPP_
#define SET_INTERVAL_FRAME_HPP_

#include "<vector>"
#include "interval_frame.hpp"
#include "interval_frame_data.hpp"
#include "time_window.hpp"
#include "temp_window.hpp"

/**
 * @brief
 * Used for interval setting.
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



#endif /* SET_INTERVAL_FRAME_HPP_ */
