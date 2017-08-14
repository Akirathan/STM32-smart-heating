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
 * @brief
 * This class controls heating. Loads interval data from @ref EEPROM in
 * constructor and periodically checks if actual temperature corresponds
 * to the temperature that was preset by the user via @ref IntervalFrame.
 *
 * @note
 * @ref MainFrame signals change in EEPROM data via @ref reloadIntervalData
 * method.
 *
 * @note
 * Implemented as singleton class.
 */
class TempController : public IMinCallback {
public:
	static TempController& getInstance();
	void operator=(const TempController&) = delete;
	TempController(const TempController&) = delete;
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
	void controlTemperature();
	void reloadIntervalData(std::vector<IntervalFrameData>& data_vec);
	uint32_t currentIntervalTemperature();
private:
	const double tempBoundary = 0.5;
	std::vector<IntervalFrameData> dataVec;
	TempController();
};

#endif /* TEMP_CONTROLLER_HPP_ */
