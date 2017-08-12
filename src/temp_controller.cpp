/*
 * temp_controller.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: mayfa
 */

#include "temp_controller.hpp"

TempController& TempController::getInstance()
{
	static TempController instance;
	return instance;
}

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
 *
 */
TempController::TempController()
{
	// Load data from EEPROM
	EEPROM& eeprom = EEPROM::getInstance();
	if (eeprom.isEmpty()) {
		// TODO Error: EEPROM supposed to contain data.
	}
	eeprom.load(dataVec);
}

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
		// TODO Error: RTC time supposed to be set.
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

void TempController::reloadIntervalData(std::vector<IntervalFrameData>& data_vec)
{
	dataVec = data_vec;
}
