/**
 * @file body.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include <cstddef> // For size_t

namespace http {

/**
 * Represents body of HTTP request or response.
 */
class Body {
public:
    static const size_t MAX_SIZE = 50;
};

} // namespace http

#endif /* BODY_HPP_ */
