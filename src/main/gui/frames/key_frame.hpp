/**
 * @file key_frame.hpp
 * @author Pavel Marek
 * @date Jul 6, 2018
 */

#ifndef KEY_FRAME_HPP_
#define KEY_FRAME_HPP_

#include "frame.hpp"
#include "callbacks.hpp" // For IExitMessageCallback
#include "hex_char_window.hpp"
#include "button.hpp"
#include "des_key.hpp"

class KeyFrame : public IFrame, public IExitMessageCallback {
public:
	KeyFrame();
	virtual void passControl() override;
	virtual void exitMessageCallback() override;
	virtual void registerExitMessageCallback() override;
	DesKey getKey();
private:
	static const size_t HEX_CHAR_WINDOWS_SIZE = 16;

	virtual void drawHeader() override;

	HexCharWindow hexCharWindows[HEX_CHAR_WINDOWS_SIZE];
	Button submitButton;
};


#endif /* KEY_FRAME_HPP_ */
