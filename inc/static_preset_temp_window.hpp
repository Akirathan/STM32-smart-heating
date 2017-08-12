/*
 * static_preset_temp_window.hpp
 *
 *  Created on: Aug 9, 2017
 *      Author: mayfa
 */

#ifndef STATIC_PRESET_TEMP_WINDOW_HPP_
#define STATIC_PRESET_TEMP_WINDOW_HPP_

#include <vector>
#include "temp_controller.hpp"
#include "static_temp_window.hpp"
#include "callbacks.hpp"
#include "temp_controller.hpp"
#include "rtc_controller.hpp"

/**
 * Used in mainframe to display temperature that is preset due to values
 * stored in EEPROM. Minute callback checks for supposed temperature from
 * TempController.
 */
class StaticPresetTempWindow : public StaticTempWindow, public IMinCallback
{
public:
	StaticPresetTempWindow(const Coord& c = Coord(0,0));
	~StaticPresetTempWindow();
	virtual void minCallback() override;
	virtual void registerMinCallback() override;
	void showPresetTemp();
private:
	bool registeredCallback = false;
};



#endif /* STATIC_PRESET_TEMP_WINDOW_HPP_ */
