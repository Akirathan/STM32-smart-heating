/**
 * @file application.hpp
 * @author mayfa
 * @date Mar 30, 2018
 */

#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "frame.hpp"
#include "key_frame.hpp"
#include "connect_frame.hpp"
#include "clk_frame.hpp"
#include "main_frame.hpp"
#include "set_interval_frame.hpp"
#include "callbacks.hpp"
#include "measured_temp_event.hpp"
#include "intervals_changed_stm_event.hpp"
#include "intervals_changed_server_event.hpp"
#include "connected_event.hpp"
#include "eth_link_up_event.hpp"
#include "communication_error_event.hpp"
#include "key_set_event.hpp"
#include "communication_device.hpp"
#include "sw_timer_owner.hpp"

/**
 * This class contains main application logic in its @ref run method.
 * This method contains indefinite loop.
 *
 * Serves as authority for dispatching global event through emitEvent methods.
 * These methods should be called by whoever generated the event.
 */
class Application : public IFrameTerminateCallback {
public:
	Application();
	static void setCurrFrame(IFrame* frame);
	virtual void frameTerminateCallback() override;
	virtual void registerFrameTerminateCallback() override;
	void switchCurrFrameToMain();
	void run();
	static uint32_t getCurrTimestamp();
	static bool isConnectedToServer();
	static bool isTimeSynced();
	static void registerSwTimerOwnerForPolling(SwTimerOwner *timer_owner);
	static void emitEvent(const ConnectedEvent &event);
	static void emitEvent(const MeasuredTempEvent &event);
	static void emitEvent(const IntervalsChangedStmEvent &event);
	static void emitEvent(const IntervalsChangedServerEvent &event);
	static void emitEvent(const CommunicationErrorEvent &event);
	static void emitEvent(const EthLinkUpEvent &event);
	static void emitEvent(const KeySetEvent &event);
private:
	static IFrame* currFrame;
	static bool clearDisplayFlag;
	static CommunicationDevice communicationDevice;
	/// These intervals are set in emitEvent(IntervalsChangedEvent) when time is
	/// not synchronized. They are pending until the device is connected to the
	/// server and time is synchronized - after this timestamp of pendingIntervals
	/// is fixed and they are sent to the server.
	static IntervalList pendingIntervals;
	static SwTimerOwner * swTimerOwners[SW_TIMERS_NUM];
	static size_t swTimerOwnersIdx;

	KeyFrame keyFrame;
	ConnectFrame connectFrame;
	ClkFrame clkFrame;
	MainFrame mainFrame;
	SetIntervalFrame setIntervalFrame;

	static void updateIntervalsMetadataInEEPROM(const ConnectedEvent &event);

	void guiTask();
	void pollSwTimers();
};



#endif /* APPLICATION_HPP_ */
