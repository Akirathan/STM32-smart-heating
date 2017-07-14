/*
 * window_system.hpp
 *
 *  Created on: Jul 13, 2017
 *      Author: mayfa
 */

#ifndef INC_WINDOW_SYSTEM_HPP_
#define INC_WINDOW_SYSTEM_HPP_

#include <memory>
#include <vector>
#include "interval_window.hpp"
#include "input.hpp"

using window_id = uint32_t;

class WindowSystem {
public:
	AppStatus_TypeDef pass_control();
	window_id add_control(std::unique_ptr<ControlWindow> window);
	window_id add_static(std::unique_ptr<StaticWindow> window);
	ControlWindow & get_control(window_id ctrl_id) const;
	WindowSystem();
private:
	friend class Windows;

	/*** Windows ***/
	class Windows {
		friend class WindowSystem;
	public:
		Windows(WindowSystem &system);
		void previous();
		void next();
		window_id add_control(std::unique_ptr<ControlWindow> window);
		window_id add_static(std::unique_ptr<StaticWindow> window);
	private:
		WindowSystem &system;
		size_t ctrl_window_idx_;
		std::vector<std::unique_ptr<ControlWindow>> ctrlWindows_;
		std::vector<std::unique_ptr<StaticWindow>> staticWindows_;

		size_t ctrl_window_idx_get() const;
		void ctrl_window_idx_inc();
		void ctrl_window_idx_dec();
	} windows_;
	/******/

	ControlWindow *curr_window_;
};


#endif /* INC_WINDOW_SYSTEM_HPP_ */
