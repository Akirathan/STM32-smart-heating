/**
 * @file interval_frame.hpp
 * @author Pavel Marek
 * @date 23.2.2017
 */

#ifndef INC_INTERVALFRAME_HPP_
#define INC_INTERVALFRAME_HPP_

#include "button.hpp"
#include "stm3210c_eval.h"
#include "lcd.hpp"
#include "frame.hpp"
#include "callbacks.hpp"

/**
 * @brief Base class for @ref SetIntervalFrame and @ref OverviewIntervalFrame
 * 		  classes.
 *
 * This frame consists of two time windows where user can set the interval time
 * and one @ref TempWindow when user can set temperature for that interval.
 * This base class provides only "NEXT" and "END" buttons, @ref SetIntervalFrame
 * adds settable time and temp windows, whereas @ref OverviewIntervalFrame
 * adds non-settable windows.
 */
class IntervalFrame : public IFrame {
public:
	IntervalFrame();
	virtual ~IntervalFrame() = default;
	void registerFrameTerminateCallbackReceiver(IFrameTerminateCallback *receiver);
protected:
	Button nextButton;
	Button endButton;
	void drawHeader();
};


#endif /* INC_INTERVALFRAME_HPP_ */
