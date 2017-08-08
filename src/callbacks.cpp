/*
 * callbacks.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: mayfa
 */

#include "callbacks.hpp"

void ISecCallback::registerSecCallback()
{
	RTCController::getInstance().registerSecondCallback(this);
}

void IMinCallback::registerMinCallback()
{
	RTCController::getInstance().registerMinuteCallback(this);
}
