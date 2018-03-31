/**
 * @file frame.cpp
 * @author mayfa
 * @date Apr 14, 2018
 */

#include "frame.hpp"
#include "application.hpp"

/**
 * Sets all the windows and header for redrawing from GUI task.
 */
void IFrame::setForRedraw()
{
	headerRedrawed = false;
	windowSystem.setForRedraw();
}
