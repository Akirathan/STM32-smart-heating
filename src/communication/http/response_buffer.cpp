/**
 * @file response_buffer.cpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#include "response_buffer.hpp"
#include <cstdlib>
#include <cctype> // For std::isspace
#include "client.hpp"
#include "tcp_driver.hpp"

namespace http {

char   ResponseBuffer::buffer[BUFF_LEN];
size_t ResponseBuffer::bufferIdx = 0;
bool   ResponseBuffer::awaitBodyFlag = false;

void ResponseBuffer::init()
{
    reset();
}

/**
 * Buffs part of HTTP message. Given buffer does not have to contain whole HTTP response.
 * Part of messages are saved into ResponseBuffer and when whole message is collected,
 * @ref Client::receiveCb is called.
 * @param part_buffer    ... buffer containing part of HTTP response.
 * @param part_buff_size ... size of the buffer
 */
void ResponseBuffer::buff(const uint8_t *part_buff, const size_t part_buff_size)
{
    if (part_buff_size + bufferIdx >= BUFF_LEN) {
        // TODO: Error: reset connection (do some logging)
        return;
    }
    std::memcpy(buffer + bufferIdx, part_buff, part_buff_size);
    bufferIdx += part_buff_size;

    Response response;
    if (parse(buffer, bufferIdx, &response)) {
        reset();
        TcpDriver::wholeMessageReceivedCb();
        Client::receiveCb(response);
    }
}

/**
 * Sets the ResponseBuffer to await also body from server. That means that message
 * that contains just header without body is considered to be incorrect by
 * ResponseBuffer.
 */
void ResponseBuffer::awaitBody()
{
    awaitBodyFlag = true;
}

/**
 * Parses HTTP response.
 * @param buffer      ... buffer to be parsed
 * @param buffer_size ... size of the buffer
 * @param response    ... pointer to an object that will be filled
 * @return false if parsing failed, true on success.
 */
bool ResponseBuffer::parse(const char *buffer, const size_t buffer_size, Response *response)
{
    bool ret_val = true;
	CharStream char_stream(buffer, buffer_size);

    int status_code = 0;
    if (!parseStatusLine(char_stream, &status_code)) {
        ret_val = false;
    }
    response->setStatusCode(static_cast<unsigned int>(status_code));

    Header header;
    if (!parseHeader(char_stream, &header)) {
        ret_val = false;
    }
    response->setHeader(header);

    // Read empty line (divider after header).
    char_stream.readWhiteSpaces();

    const char *body = char_stream.getRestOfBuffer();
    const size_t body_len = char_stream.getBufferSize() - char_stream.getBufferIdx();

    if (body == nullptr && awaitBodyFlag) {
        ret_val = false;
    }

    if (body != nullptr) {
        response->copyIntoBody(reinterpret_cast<const uint8_t *>(body), body_len);
    }

    return ret_val;
}

/**
 * Parses whole status line
 * @param char_stream
 * @return status code in int
 */
bool ResponseBuffer::parseStatusLine(CharStream &char_stream, int *status_code)
{
    bool ret_val = true;
    char status_line[3][20];
    char_stream.readLine(status_line, nullptr);

    // Convert status code into int.
    *status_code = std::atoi(status_line[1]);
    if (*status_code == 0) {
        ret_val = false;
    }
    return ret_val;
}

/**
 * Parses whole header.
 * @param char_stream
 * @param header [out] ... filled with correct options ie. not not-unknown-type options.
 * @return
 */
bool ResponseBuffer::parseHeader(CharStream &char_stream, Header *header)
{
    bool ret_val = true;
    while (!char_stream.atEmptyLine() && !char_stream.atEnd()) {
        HeaderOption header_option;
        if (!parseOptionLine(char_stream, &header_option)) {
            ret_val = false;
            break;
        }

        if (header_option.getType() != HeaderOption::UNKNOWN_TYPE) {
            header->appendOption(header_option);
        }
    }
    return ret_val;
}

/**
 * Parses one line
 * @param header_option [out] ... may be filled as unknown header option
 * @return false when parsing failed
 */
bool ResponseBuffer::parseOptionLine(CharStream &char_stream, HeaderOption *header_option)
{
    bool ret_val = true;
    char words[2][MAX_OPTION_WORD_LEN];
    size_t words_count = 0;
    char_stream.readLine(words, &words_count, ':');

    HeaderOption::Type option_type;
    resolveOptionType(trimWhiteSpaces(words[0]), &option_type);
    header_option->setType(option_type);

    // Fill value of header_option if option is not of unknown type.
    if (option_type != HeaderOption::UNKNOWN_TYPE) {
        if (words_count <= 2) {
            header_option->setValue(trimWhiteSpaces(words[1]));
        }
        else {
            ret_val = false;
        }
    }
    return ret_val;
}

void ResponseBuffer::resolveOptionType(const char *word, HeaderOption::Type *type)
{
    if (std::strcmp(word, HeaderOption::CONTENT_LENGTH_STR) == 0) {
        *type = HeaderOption::CONTENT_LENGTH;
    }
    else if (std::strcmp(word, HeaderOption::CONTENT_TYPE_STR) == 0) {
        *type = HeaderOption::CONTENT_TYPE;
    }
    else {
        *type = HeaderOption::UNKNOWN_TYPE;
    }
}

/**
 * Returns pointer to substring that is trimmed from whitespaces
 */
char *ResponseBuffer::trimWhiteSpaces(char *str)
{
    while (std::isspace(*str)) {
        str++;
    }

    char *end = str + std::strlen(str) - 1;
    while (std::isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

void ResponseBuffer::reset()
{
    for (size_t i = 0; i < BUFF_LEN; i++) {
        buffer[i] = '\0';
    }
    bufferIdx = 0;
    awaitBodyFlag = false;
}

} // namespace http
