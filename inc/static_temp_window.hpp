/*
 * static_temp_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TEMP_WINDOW_HPP_
#define INC_STATIC_TEMP_WINDOW_HPP_

#include "stm3210c_eval_lcd.h"
#include "hideable_window.hpp"
#include "rtc_controller.hpp"
#include "temp_sensor.hpp"
#include "window.hpp"
#include "lcd.hpp"
#include "callbacks.hpp"

/**
 * Base class providing draw method.
 */
class StaticTempWindow : public StaticWindow, public IHideableWindow
{
public:
	StaticTempWindow(const Coord& c = Coord(0,0));
	virtual void draw() const override;
	void hide() override;
	void show() override;
	void setTemp(double temp);
protected:
	double temp = 0;
};

/**
 * Can run on background. Use hide method to force not-drawing of this window.
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


#endif /* INC_STATIC_TEMP_WINDOW_HPP_ */
