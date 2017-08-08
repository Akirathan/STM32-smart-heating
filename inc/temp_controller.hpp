/*
 * temp_controller.hpp
 *
 *  Created on: Aug 8, 2017
 *      Author: mayfa
 */

#ifndef TEMP_CONTROLLER_HPP_
#define TEMP_CONTROLLER_HPP_

#include <vector>
#include "callbacks.hpp"
#include "rtc_controller.hpp"
#include "eeprom.hpp"
#include "relay.hpp"
#include "time.hpp"
#include "temp_sensor.hpp"

/**
 * Singleton class.
 */
class TempController : public IMinCallback {
public:
	static TempController& getInstance();
	void operator=(const TempController&) = delete;
	TempController(const TempController&) = delete;
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
	void controlTemperature();
private:
	const double tempBoundary = 0.5;
	std::vector<IntervalFrameData> dataVec;
	TempController();
	uint32_t currentIntervalTemperature();
};

#endif /* TEMP_CONTROLLER_HPP_ */
