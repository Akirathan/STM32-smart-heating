/**
 * @file response_buffer.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef RESPONSE_BUFFER_HPP_
#define RESPONSE_BUFFER_HPP_

#include <cstdint>
#include "response.hpp"
#include "header_option.hpp"
#include "header.hpp"
#include "char_stream.hpp"

namespace http {

/**
 * @brief Represents a buffer for HTTP responses that is directly called from a TCP driver.
 * (implemented as static class)
 *
 * ResponseBuffer buffs given parts of HTTP responses via @ref buff method.
 * Note that it is quite common that the responses are sent line by line
 * so the buffering is necessary.
 *
 * When the body is awaited, the application layer should call @ref awaitBody,
 * otherwise only response header will be buffered and immediately send to
 * higher layers.
 *
 * When whole response is buffered, @ref Client::receiveCb is called and also
 * TcpDriver is notified via @ref TcpDriver::wholeMessageReceivedCb so it can
 * shutdown the connection.
 */
class ResponseBuffer {
public:
    static void init();
    static void reset();
    static void buff(const uint8_t *part_buff, const size_t part_buff_size);
    static void awaitBody();
private:
    static const size_t MAX_OPTION_WORD_LEN = 50;
    static const size_t BUFF_LEN = 1500;

    static char buffer[BUFF_LEN];
    static size_t bufferIdx;
    static bool awaitBodyFlag;

    static bool parse(const char *buffer, const size_t buffer_size, Response *response);
    static bool parseStatusLine(CharStream &char_stream, int *status_code);
    static bool parseHeader(CharStream &char_stream, Header *header);
    static bool parseOptionLine(CharStream &char_stream, HeaderOption *header_option);
    static void resolveOptionType(const char *word, HeaderOption::Type *type);
    static char * trimWhiteSpaces(char *str);

    friend class ResponseBufferTest;
};

} // namespace http

#endif /* RESPONSE_BUFFER_HPP_ */
