/**
 * @file overview_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef OVERVIEW_INTERVAL_FRAME_HPP_
#define OVERVIEW_INTERVAL_FRAME_HPP_

#include "interval_frame.hpp"
#include "interval_frame_data.hpp"
#include "static_time_window.hpp"
#include "static_temp_window.hpp"
#include "time.hpp"
#include "window_system.hpp"
#include "callbacks.hpp"
#include "settings.hpp"

/**
 * @brief Used for overview of intervals.
 *
 * This frame is displayed When user presses overview button in main frame,
 * and is used just for displaying interval data eg. those data stored in
 * EEPROM. Note that all window members are static.
 */
class OverviewIntervalFrame : public IntervalFrame, IExitMessageCallback {
public:
	OverviewIntervalFrame();
	OverviewIntervalFrame(const IntervalFrameData data[], size_t count);
	void setData(const IntervalFrameData data[], size_t count);
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	virtual void passControl() override;
private:
	StaticTimeWindow timeFromWindow;
	StaticTimeWindow timeToWindow;
	StaticTempWindow tempWindow;
	IntervalFrameData data[INTERVALS_NUM];
	size_t dataIdx;
	size_t dataCount;
	void printData(const IntervalFrameData& data);
	void drawHeader() override;
};


#endif /* OVERVIEW_INTERVAL_FRAME_HPP_ */
