/**
 * @file static_preset_temp_window.hpp
 * @author Pavel Marek
 * @date 9.8.2017
 */

#ifndef STATIC_PRESET_TEMP_WINDOW_HPP_
#define STATIC_PRESET_TEMP_WINDOW_HPP_

#include "temp_controller.hpp"
#include "static_temp_window.hpp"
#include "callbacks.hpp"
#include "rtc_controller.hpp"

/**
 * @brief Static window used in @ref MainFrame to display temperature that was
 * 		  configured and stored in EEPROM.
 *
 * After @ref showPresetTemp method is called, (minute) callback checks for
 * preset temperature from @ref TempController.
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
