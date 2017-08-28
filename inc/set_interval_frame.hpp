/**
 * @file set_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef SET_INTERVAL_FRAME_HPP_
#define SET_INTERVAL_FRAME_HPP_

#include <vector>
#include "interval_frame.hpp"
#include "interval_frame_data.hpp"
#include "time_window.hpp"
#include "temp_window.hpp"
#include "time.hpp"
#include "window_system.hpp"

/**
 * @brief Used for interval setting.
 *
 * This frame is displayed when user presses SET button in main frame.
 * One should call @ref passControl and then @ref getData methods in
 * this order.
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
