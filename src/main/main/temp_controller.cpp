/**
 * @file temp_controller.cpp
 * @author Pavel Marek
 * @date 8.8.2017
 */

#include "temp_controller.hpp"

TempController& TempController::getInstance()
{
	static TempController instance;
	return instance;
}

/**
 * @brief Checks if current temperature corresponds to the temperature that was
 * 		  configured by the user in interval setting.
 *
 * If the temperature does not correspond, the heating (relay module) is
 * switched on.
 *
 * @note Switches heating (relay module) off when no interval is active.
 */
void TempController::minCallback()
{
	// Switch heating off from previous callbacks
	Relay& relay = Relay::getInstance();
	relay.switchOff();

	// Get current temperature and current interval temperature.
	TempSensor::init();
	double temp = TempSensor::measure_temperature();
	double expected_temp = currentIntervalTemperature();

	// Compare them
	if (temp - tempBoundary <= expected_temp && expected_temp <= temp + tempBoundary) {
		// Temperature inside bounds - do nothing.
	}
	else if (temp < expected_temp){
		// Turn heating on.
		relay.switchOn();
	}
	// No interval is active.
	else if (expected_temp == 0) {
		relay.switchOff();
	}
}

void TempController::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}

/**
 * @brief Loads data from @ref EEPROM.
 */
TempController::TempController()
{
	// Load data from EEPROM
	EEPROM& eeprom = EEPROM::getInstance();
	if (eeprom.isEmpty()) {
		// Error: EEPROM supposed to contain data.
		Error_Handler();
	}
	eeprom.load(dataVec);
}

/**
 * @brief Registers for minute callback.
 */
void TempController::controlTemperature()
{
	// Register for minute callback
	registerMinCallback();
}

/**
 * Returns supposed temperature at current time that was contained in EEPROM.
 */
uint32_t TempController::currentIntervalTemperature()
{
	// Get time from RTC
	RTC_TimeTypeDef curr_time {0,0,0};
	RTCController& rtc = RTCController::getInstance();
	if (!rtc.isTimeSet()) {
		// Error: RTC time supposed to be set.
		Error_Handler();
	}
	rtc.getTime(&curr_time);

	// Serialize current time.
	uint32_t curr_time_serialized = Time::serialize(Time::Time{curr_time.Hours, curr_time.Minutes});

	// Find current interval.
	for (auto it = dataVec.begin(); it != dataVec.end(); ++it) {
		if (it->from <= curr_time_serialized && curr_time_serialized <= it->to) {
			// Return temperature from current interval.
			return it->temp;
		}
	}

	// In case when no interval is active.
	return 0;
}

/**
 * @brief Reloads interval data.
 *
 * Called from @ref MainFrame to signal change in EEPROM data.
 */
void TempController::reloadIntervalData(std::vector<IntervalFrameData>& data_vec)
{
	dataVec = data_vec;
}
