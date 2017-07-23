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
#include "window.hpp"
#include "input.hpp"

class window_system {
	friend class Windows;

	/*** Windows ***/
	class windows {
		friend class window_system;
	public:
		windows(window_system &system);
		void previous();
		void next();
		void add_control(control_window *window);
		void add_static(static_window *window);
	private:
		window_system &system;
		size_t ctrl_window_idx;
		std::vector<control_window *> ctrl_windows;
		std::vector<static_window *> static_windows;

		size_t ctrl_window_idx_get() const;
		void ctrl_window_idx_inc();
		void ctrl_window_idx_dec();
	} windows_;
	/******/

	control_window *curr_window;
public:
	AppStatus_TypeDef pass_control();
	void add_control(control_window *window);
	void add_static(static_window *window);
	window_system();
};


#endif /* INC_WINDOW_SYSTEM_HPP_ */
