/**
 * @file header.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef HEADER_HPP_
#define HEADER_HPP_

#include "header_option.hpp"

namespace http {

/**
 * @brief Represents HTTP header.
 *
 * @note Header may be empty ie. contain no @ref HeaderOption.
 */
class Header {
private:
    static const size_t MAX_OPTIONS = 10;

public:
    static const size_t MAX_SIZE = MAX_OPTIONS * HeaderOption::MAX_SIZE;

	Header();
	void appendOption(const HeaderOption &option);
	const char *getOptionValueByType(HeaderOption::Type type) const;
	size_t getTotalSize() const;
	void toBuffer(char *buffer) const;

private:
	HeaderOption options[MAX_OPTIONS];
	size_t optionsIdx;
};

} // namespace http


#endif /* HEADER_HPP_ */
