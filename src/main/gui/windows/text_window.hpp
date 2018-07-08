/**
 * @file text_window.hpp
 * @author Pavel Marek
 * @date Jul 7, 2018
 */

#ifndef TEXT_WINDOW_HPP_
#define TEXT_WINDOW_HPP_

#include "static_window.hpp" // For IStaticWindow

class TextWindow : public IStaticWindow {
public:
	TextWindow();
	TextWindow(const Coord &coord, const char *text);
	void setText(const char *text);
	const char * getText() const;
private:
	static const size_t TEXT_LEN = 15;

	char text[TEXT_LEN];
	char oldText[TEXT_LEN];

	virtual void draw() const override;
	void clearOldText() const;
};

#endif /* TEXT_WINDOW_HPP_ */
