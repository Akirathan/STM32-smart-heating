/**
 * @file static_measure_temp_window.hpp
 * @author Pavel Marek
 * @date 9.8.2017
 */

#ifndef STATIC_MEASURE_TEMP_WINDOW_HPP_
#define STATIC_MEASURE_TEMP_WINDOW_HPP_

#include "static_temp_window.hpp"
#include "callbacks.hpp"
#include "rtc_controller.hpp"
#include "temp_sensor.hpp"

/**
 * @brief Static window for displaying actual temperature.
 *
 * After @ref measure method is called, temperature is measured in every minute
 * callback (@ref minCallback) and the current temperature is then displayed.
 *
 * @note Note that this window can run on background.
 */
class StaticMeasureTempWindow : public StaticTempWindow, public IMinCallback
{
public:
	StaticMeasureTempWindow(const Coord& c = Coord(0,0));
	~StaticMeasureTempWindow();
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
	void measure();
private:
	bool registeredCallback = false;
};


#endif /* STATIC_MEASURE_TEMP_WINDOW_HPP_ */
