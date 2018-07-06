/**
 * @file key_frame.cpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#include "key_frame.hpp"

KeyFrame::KeyFrame()
{

}

void KeyFrame::passControl()
{

}

void KeyFrame::exitMessageCallback()
{
	windowSystem.unregisterExitMessageCallbackReceiver(this);
	windowSystem.stop();
	callTerminateCallbackReceivers();
}

void KeyFrame::registerExitMessageCallback()
{
	// Intentionally left empty.
}

