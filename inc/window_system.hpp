/*
 * window_system.hpp
 *
 *  Created on: Jul 13, 2017
 *      Author: mayfa
 */

#ifndef INC_WINDOW_SYSTEM_HPP_
#define INC_WINDOW_SYSTEM_HPP_

#include <vector>
#include "window.hpp"
#include "input.hpp"

class WindowSystem {
private:
	class Windows {
	public:
		Windows(WindowSystem &system);
		void previous();
		void next();
		void addControl(ControlWindow* window);
		void addStatic(StaticWindow* window);
	private:
		friend class WindowSystem;
		WindowSystem& system;
		size_t ctrlWindowIdx;
		std::vector<ControlWindow *> ctrlWindows;
		std::vector<StaticWindow *> staticWindows;
		size_t ctrlWindowIdxGet() const;
		void ctrlWindowIdxInc();
		void ctrlWindowIdxDec();
	};
public:
	AppStatus_TypeDef passControl();
	void addControl(ControlWindow* window);
	void addStatic(StaticWindow* window);
	WindowSystem();
private:
	friend class Windows;
	Windows windows;
	ControlWindow* currWindow;
};


#endif /* INC_WINDOW_SYSTEM_HPP_ */
