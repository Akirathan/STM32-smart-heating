/**
 * @file temp_controller.hpp
 * @author Pavel Marek
 * @date 8.8.2017
 */

#ifndef TEMP_CONTROLLER_HPP_
#define TEMP_CONTROLLER_HPP_

#include "callbacks.hpp"
#include "rtc_controller.hpp"
#include "eeprom.hpp"
#include "relay.hpp"
#include "time.hpp"
#include "temp_sensor.hpp"
#include "settings.h"

/**
 * @brief This class controls heating.
 *
 * Loads interval data from @ref EEPROM in constructor and every minute
 * checks if actual temperature corresponds to the temperature that was
 * configured by the user in @ref IntervalFrame.
 *
 * @attention @ref controlTemperature method must be called to register for
 * 			  minute callbacks.
 *
 * @note @ref MainFrame signals change in EEPROM data via @ref reloadIntervalData
 * 		 method.
 *
 * @note Implemented as a singleton class.
 */
class TempController : public IMinCallback {
public:
	static TempController& getInstance();
	void operator=(const TempController&) = delete;
	TempController(const TempController&) = delete;
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
	void controlTemperature();
	void reloadIntervalData(const IntervalFrameData data[], size_t count);
	uint32_t currentIntervalTemperature();
private:
	const double tempBoundary = 0.5;
	IntervalFrameData data[INTERVALS_NUM];
	size_t dataCount;
	TempController();
};

#endif /* TEMP_CONTROLLER_HPP_ */
