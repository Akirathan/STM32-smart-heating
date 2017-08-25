/*
 * static_temp_window.hpp
 *
 *  Created on: Jul 21, 2017
 *      Author: mayfa
 */

#ifndef INC_STATIC_TEMP_WINDOW_HPP_
#define INC_STATIC_TEMP_WINDOW_HPP_

#include "hideable_window.hpp"
#include "window.hpp"
#include "lcd.hpp"

/**
 * @brief
 * Class representing static window just for showing temperature to user.
 * @note
 * Note that this class is also base class for @ref StaticMeasureTempWindow
 * and @ref StaticPresetTempWindow so those derived classes do not have to
 * implement @ref draw method.
 */
class StaticTempWindow : public IStaticWindow, public IHideableWindow
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


#endif /* INC_STATIC_TEMP_WINDOW_HPP_ */
