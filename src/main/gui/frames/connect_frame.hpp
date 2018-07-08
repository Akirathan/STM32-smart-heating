/**
 * @file connect_frame.hpp
 * @author Pavel Marek
 * @date Jul 2, 2018
 */

#ifndef CONNECT_FRAME_HPP_
#define CONNECT_FRAME_HPP_

#include "frame.hpp"
#include "button.hpp"
#include "callbacks.hpp" // For IExitMessageCallback

class ConnectFrame : public IFrame, public IExitMessageCallback {
public:
	ConnectFrame();
	virtual void passControl() override;
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	bool yesButtonPushed() const;
private:
	Button yesButton;
	Button noButton;

	virtual void drawHeader() override;
};

#endif /* CONNECT_FRAME_HPP_ */
