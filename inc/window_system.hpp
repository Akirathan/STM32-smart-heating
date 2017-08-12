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
		void addControl(IControlWindow* window);
		void addStatic(IStaticWindow* window);
	private:
		friend class WindowSystem;
		WindowSystem& system;
		size_t ctrlWindowIdx;
		std::vector<IControlWindow *> ctrlWindows;
		std::vector<IStaticWindow *> staticWindows;
		size_t ctrlWindowIdxGet() const;
		void ctrlWindowIdxInc();
		void ctrlWindowIdxDec();
	};
public:
	AppStatus_TypeDef passControl();
	void addControl(IControlWindow* window);
	void addStatic(IStaticWindow* window);
	WindowSystem();
private:
	friend class Windows;
	Windows windows;
	IControlWindow* currWindow;
};


#endif /* INC_WINDOW_SYSTEM_HPP_ */
