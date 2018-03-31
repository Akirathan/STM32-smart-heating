/**
 * @file set_interval_frame.hpp
 * @author Pavel Marek
 * @date 25.8.2017
 */

#ifndef SET_INTERVAL_FRAME_HPP_
#define SET_INTERVAL_FRAME_HPP_

#include "callbacks.hpp"
#include "interval_frame.hpp"
#include "interval_frame_data.hpp"
#include "settings.h"
#include "time_window.hpp"
#include "temp_window.hpp"
#include "time.hpp"
#include "window_system.hpp"

/**
 * @brief Used for interval setting.
 *
 * This frame is displayed when user presses SET button in main frame,
 * or when no EEPROM interval data are available after @ref ClkFrame.
 * One should call @ref passControl and then @ref getData methods in
 * this order.
 */
class SetIntervalFrame : public IntervalFrame, IExitMessageCallback {
public:
	SetIntervalFrame();
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	void getData(IntervalFrameData data[], size_t* count);
	virtual void passControl() override;
private:
	TimeWindow timeFromWindow;
	TimeWindow timeToWindow;
	TempWindow tempWindow;
	IntervalFrameData data[INTERVALS_NUM];
	size_t dataCount;
	void processInterval();
	void drawHeader() override;
};


#endif /* SET_INTERVAL_FRAME_HPP_ */
