/**
 * @file char_stream.cpp
 * @author Pavel marek
 * @date Jun 25, 2018
 */

#include "char_stream.hpp"
#include <cstring>

CharStream::CharStream(const char *buffer, const size_t buffer_size) :
    buffer(buffer),
    bufferSize(buffer_size),
    bufferIdx(0)
{}

const size_t CharStream::getBufferSize() const
{
    return bufferSize;
}

size_t CharStream::getBufferIdx() const
{
    return bufferIdx;
}

char CharStream::readChar()
{
    if (bufferIdx >= bufferSize) {
        return '\0';
    }
    return buffer[bufferIdx++];
}

/**
 * Reads one word
 * @param word     ... string where read word will be stored
 * @param word_len ... may be nullptr, otherwise length of read word is assigned.
 */
void CharStream::readWord(char *word, size_t *word_len)
{
    char c = '\0';
    unsigned long wordIdx = 0;
    while (!isWhiteSpace(c) && !atEnd()) {
        c = readChar();
        if (!isWhiteSpace(c)) {
            word[wordIdx++] = c;
        }
    }

    word[wordIdx++] = '\0';

    if (!atEnd()) {
        unreadChar();
    }

    if (word_len != nullptr) {
        *word_len = wordIdx;
    }
}

/**
 * Reads one word until delimiter. After reading, next char points at delimiter.
 * @param delimiter
 */
void CharStream::readUntilDelimiter(char *word, size_t *word_len, const char delimiter)
{
    char c = '\0';
    unsigned long wordIdx = 0;
    while (c != delimiter && !atEndOfLine() && !atEnd()) {
        c = readChar();
        if (c != delimiter) {
            word[wordIdx++] = c;
        }
    }

    word[wordIdx++] = '\0';

    if (!atEnd()) {
        unreadChar();
    }

    if (word_len != nullptr) {
        *word_len = wordIdx;
    }
}

void CharStream::readWhiteSpaces()
{
    char c = ' ';
    while (isWhiteSpace(c)) {
        c = readChar();
    }
    if (c != '\0') {
        unreadChar();
    }
}

/**
 * Reads \r\n. If there is some unexpected data in buffer, returns false.
 */
bool CharStream::readLineDelimiter()
{
	char r = readChar();
	char n = readChar();
	if (r != '\r' || n != '\n') {
		return false;
	}
	return true;
}

bool CharStream::atEnd() const
{
    return bufferIdx >= bufferSize;
}

/**
 * Returns pointer to the rest of the buffer.
 * Used for getting body of HTTP message for example.
 *
 * @return pointer to the rest of the buffer
 */
const char * CharStream::getRestOfBuffer()
{
    const char *ret_val = nullptr;
    if (!atEnd()) {
        ret_val = buffer + bufferIdx;
    }
    return ret_val;
}

bool CharStream::atEmptyLine() const
{
    return atEndOfLine();
}

bool CharStream::isWhiteSpace(char c) const
{
    return c == '\r' || c == '\n' || c == ' ' || c == '\t';
}

void CharStream::unreadChar()
{
    bufferIdx--;
}

bool CharStream::atEndOfLine() const
{
    bool ret_val = false;
    if (bufferIdx < bufferSize - 2) {
        ret_val = buffer[bufferIdx] == '\r' && buffer[bufferIdx + 1] == '\n';
    }
    return ret_val;
}

void CharStream::shiftBuffer(size_t count)
{
    if (bufferIdx + count < bufferSize) {
        bufferIdx += count;
    }
    else {
        bufferIdx = bufferSize;
    }
}

