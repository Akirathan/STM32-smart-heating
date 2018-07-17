/**
 * @file temp_window.hpp
 * @author Pavel Marek
 * @date 21.7.2017
 */

#ifndef INC_TEMP_WINDOW_HPP_
#define INC_TEMP_WINDOW_HPP_

#include "control_window.hpp"
#include "lcd.hpp"

/**
 * @brief Control window for temperature configuration.
 *
 * The value cannot exceed interval given by @ref highBond and @ref lowBond.
 */
class TempWindow : public IControlWindow {
public:
	TempWindow(const Coord& coord = Coord(0,0));
	void draw()const override;
	void setFocus(Message msg) override;
	uint32_t getTemp() const;
	void setTemp(uint32_t temp);
private:
	uint32_t temp;
	// Temperature higher boundary
	static const uint32_t highBond = 30;
	// Temperature lower boundary
	static const uint32_t lowBond = 8;
	bool focused = false;
	Message _eventHandler(JOYState_TypeDef joy_state) override;
	void unsetFocus();
};


#endif /* INC_TEMP_WINDOW_HPP_ */
