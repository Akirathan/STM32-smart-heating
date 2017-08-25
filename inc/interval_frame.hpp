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
