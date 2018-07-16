/**
 * @file request.cpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#include "request.hpp"
#include <cstring>
#include "rt_assert.h"

namespace http {

Request::Request(Request::method_t method, const char *url) :
        method(method),
        version("HTTP/1.1"),
        bodyLen(0)
{
    std::strcpy(this->url, url);

    for (size_t i = 0; i < Body::MAX_SIZE; ++i) {
        body[i] = '\0';
    }
}

/**
 * Returns total size in bytes of this Request object.
 */
size_t Request::getSize() const
{
	size_t size = 0;
    size += getFirstLineSize();
    size += header.getTotalSize();
    if (bodyLen > 0) {
        // Empty line (delimiter) size + header size
        size += 2;
        size += bodyLen;
    }
    return size;
}

Header & Request::getHeader()
{
	return header;
}

const uint8_t * Request::getBody() const
{
	return body;
}

size_t Request::getBodyLen() const
{
	return bodyLen;
}

/**
 * Copies given header into this Request object.
 */
void Request::appendHeader(const Header &header)
{
    this->header = header;
}

/**
 * Copies given buffer into the body of this Request object.
 * @param buff      ... buffer that will be copied into body.
 * @param buff_size ... size of the buffer
 */
void Request::appendBody(const uint8_t *buff, const size_t buff_size)
{
	rt_assert(buff_size <= Body::MAX_SIZE, "buffer overflow");

    std::memcpy(body, buff, buff_size);
    bodyLen = buff_size;
}

/**
 * @brief Serializes this whole Request into given buffer.
 *
 * Before calling this method, make sure that this Request object is complete (ie.
 * @ref appendHeader and @ref appendBody were already called), otherwise the given
 * buffer would be filled with nonsense data.
 *
 * @param buffer ... buffer that will be filled with data of this Request object.
 */
void Request::toBuffer(char *buffer) const
{
    firstLineToBuffer(buffer);
    buffer += getFirstLineSize();
    header.toBuffer(buffer);
    buffer += header.getTotalSize();
    if (bodyLen > 0) {
        *(buffer++) = '\r';
        *(buffer++) = '\n';
        std::memcpy(buffer, body, bodyLen);
        *(buffer + bodyLen) = '\0';
    }
}

size_t Request::getFirstLineSize() const
{
    size_t size = 0;
	switch (method) {
        case GET:
            size += 3;
            break;
        case POST:
            size += 4;
            break;
    }
    size += 1;
    size += std::strlen(url);
    size += 1;
    size += std::strlen(version);
    // Add line break size \r\n
    size += 2;
    return size;
}

void Request::firstLineToBuffer(char *buffer) const
{
    switch(method) {
        case GET:
            std::strcpy(buffer, "GET");
            buffer += std::strlen("GET");
            break;
        case POST:
            std::strcpy(buffer, "POST");
            buffer += std::strlen("POST");
            break;
    }
    *(buffer++) = ' ';
    std::strcpy(buffer, url);
    buffer += std::strlen(url);

    *(buffer++) = ' ';
    std::strcpy(buffer, version);
    buffer += std::strlen(version);

    *(buffer++) = '\r';
    *(buffer++) = '\n';
    *buffer = '\0';
}

} // namespace http
