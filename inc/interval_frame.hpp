/*
 * interval_frame.hpp
 *
 *  Created on: 23. 2. 2017
 *      Author: Mayfa
 */

#ifndef INC_INTERVALFRAME_HPP_
#define INC_INTERVALFRAME_HPP_

#include "button.hpp"
#include "stm3210c_eval.h"
#include "lcd.hpp"
#include "frame.hpp"

/**
 * @brief
 * Base class for @ref SetIntervalFrame and @ref OverviewIntervalFrame classes.
 * This frame consists of two time windows where user can set the interval time
 * and one @ref TempWindow when user can set temperature for that interval.
 * This base provides only "NEXT" and "END" buttons, @ref SetIntervalFrame
 * adds settable time and temp windows, whereas @ref OverviewIntervalFrame
 * adds non-settable windows.
 */
class IntervalFrame : public IFrame {
public:
	IntervalFrame();
	virtual ~IntervalFrame() = default;
protected:
	Button nextButton;
	Button endButton;
	void drawHeader();
};



#endif /* INC_INTERVALFRAME_HPP_ */
