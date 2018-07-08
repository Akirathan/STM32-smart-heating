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
#include "key_frame.hpp"
#include "eeprom.hpp"
#include "static_time_window.hpp"
#include "static_measure_temp_window.hpp"
#include "static_preset_temp_window.hpp"
#include "text_window.hpp"
#include "window_system.hpp"
#include "lcd.hpp"
#include "temp_controller.hpp"
#include "frame.hpp"
#include "callbacks.hpp"
#include "sw_timer_owner.hpp"

/**
 * @brief Main frame of the application.
 *
 * This frame is displayed after @ref ClkFrame and @ref SetIntervalFrame.
 * That means that time is set in @ref RTCController and interval frame data
 * are stored in @ref EEPROM.
 *
 * Note that user can access setIntervalFrame or overviewIntervalFrame with
 * resetButton or overviewButton. In those cases main frame is not displayed.
 */
class MainFrame : public IFrame, IFrameTerminateCallback, IExitMessageCallback, SwTimerOwner {
public:
	MainFrame();
	virtual void passControl() override;
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	virtual void frameTerminateCallback() override;
	virtual void registerFrameTerminateCallback() override;
	virtual void timeout() override;
private:
	/**
	 * Timer timeout in milliseconds, for more information see @ref SwTimerOwner
	 */
	static const unsigned int TIMER_TIMEOUT = 2000;

	/**
	 * Which frame is currently displayed.
	 */
	enum {
		NONE,
		SET_INTERVAL_FRAME,
		OVERVIEW_INTERVAL_FRAME,
		KEY_FRAME
	} currFrameType;

	enum status_t {
		UNKNOWN,
		OFFLINE,       // ETH link is down, cannot connect
		LINK_UP,       // ETH link up, but key is not in EEPROM
		CONNECTING,
		CONNECTED      // STM is fully connected to the server
	} connectedStatus;

	StaticTimeWindow timeWindow;
	StaticMeasureTempWindow actualTempWindow;
	StaticPresetTempWindow presetTempWindow;
	Button overviewButton;
	Button resetButton;
	Button connectButton;
	TextWindow statusTextWindow;
	SetIntervalFrame setIntervalFrame;
	OverviewIntervalFrame overviewIntervalFrame;
	KeyFrame keyFrame;
	bool callbackRegistered;
	bool connectButtonInWindowSystem;

	void drawHeader() override;
	void updateStatus();
	void setOfflineStatus();
	void setLinkUpStatus();
	void setConnectingStatus();
	void setConnectedStatus();
	void showConnectButton();
	void hideConnectButton();
};


#endif /* INC_MAINFRAME_HPP_ */
