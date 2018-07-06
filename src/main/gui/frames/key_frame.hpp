/**
 * @file key_frame.hpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#ifndef KEY_FRAME_HPP_
#define KEY_FRAME_HPP_

#include "frame.hpp"
#include "callbacks.hpp" // For IExitMessageCallback

class KeyFrame : public IFrame, public IExitMessageCallback {
public:
	KeyFrame();
	virtual void passControl() override;
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
};


#endif /* KEY_FRAME_HPP_ */
