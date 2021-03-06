/**
 * @file mainframe.hpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#ifndef INC_MAINFRAME_HPP_
#define INC_MAINFRAME_HPP_

#include <button.hpp>
#include "overview_interval_frame.hpp"
#include "set_interval_frame.hpp"
#include "eeprom.hpp"
#include "static_time_window.hpp"
#include "static_measure_temp_window.hpp"
#include "static_preset_temp_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"
#include "temp_controller.hpp"
#include "frame.hpp"

/**
 * @brief Main frame of the application.
 *
 * This frame is displayed after @ref ClkFrame and @ref SetIntervalFrame.
 * That means that time is set in @ref RTCController and interval frame data
 * are stored in @ref EEPROM.
 */
class MainFrame : public IFrame {
public:
	MainFrame();
	virtual void passControl() override;
private:
	StaticTimeWindow timeWindow;
	StaticMeasureTempWindow actualTempWindow;
	StaticPresetTempWindow presetTempWindow;
	Button overviewButton;
	Button resetButton;
	void drawHeader();
};


#endif /* INC_MAINFRAME_HPP_ */
