/**
 * @file body.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include <cstddef> // For size_t
#include "settings.hpp" // For INTERVALS_NUM
#include "interval.hpp" // For Interval::SIZE

namespace http {

/**
 * Represents body of HTTP request or response.
 */
class Body {
public:
	/**
	 * "POST intervals" is the message with the biggest body. Some overhead is
	 * also considered.
	 */
    static const size_t MAX_SIZE = INTERVALS_NUM * Interval::SIZE + 30;
};

} // namespace http

#endif /* BODY_HPP_ */
