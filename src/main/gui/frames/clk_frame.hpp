/**
 * @file clkframe.hpp
 * @author Pavel Marek
 * @date 4.2.2017
 */

#ifndef CLK_FRAME_HPP_
#define CLK_FRAME_HPP_

#include "stm3210c_eval.h"
#include <button.hpp>
#include "time_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"
#include "frame.hpp"
#include "callbacks.hpp"

/**
 * @brief Represents class for user time selection.
 *
 * The user time selection consists of two windows - clock (hours : minutes)
 * and ok button. This frame ends when ok button is pressed.
 */
class ClkFrame : public IFrame, IExitMessageCallback {
public:
	ClkFrame();
	virtual void passControl() override;
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	RTC_TimeTypeDef getTime() const;
private:
	void drawHeader() override;
	TimeWindow time;
	Button ok_button;
};

#endif /* CLK_FRAME_HPP_ */
