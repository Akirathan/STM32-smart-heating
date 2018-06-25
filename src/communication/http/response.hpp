/**
 * @file response.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <cstddef>
#include <cstdint>
#include "header.hpp"
#include "body.hpp"

namespace http {

/**
 * @brief Represents immutable HTTP response object.
 *
 * @note Body may be arbitrary binary value not only text.
 * @note Currently only few status codes are supported.
 */
class Response {
public:
	enum status_code_t {
        NONE,
		FORBIDDEN,
		OK
	};

    Response();
    void setStatusCode(unsigned int status_code);
	status_code_t getStatusCode() const;
    void copyIntoBody(const uint8_t *buff, const size_t buff_size);
    void setHeader(const Header &header);
	size_t getBodySize() const;
	const uint8_t *getBody() const;
private:
	status_code_t statusCode;
    Header header;
	uint8_t body[Body::MAX_SIZE];
    size_t bodySize;

    status_code_t convertStatusCode(unsigned int code);
};

} // namespace http

#endif /* RESPONSE_HPP_ */
