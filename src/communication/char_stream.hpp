/**
 * @file char_stream.hpp
 * @author Pavel Marek
 * @date Jun 25, 2018
 */

#ifndef CHAR_STREAM_HPP_
#define CHAR_STREAM_HPP_

#include <cstddef>
#include <cstring>

/**
 * @brief This class is wrapper for C string and provides "stream" methods.
 *
 * @note line delimiter is considered to be \r\n (just like in HTTP).
 */
class CharStream {
public:
	CharStream(const char *buffer, const size_t buffer_size);
    const size_t getBufferSize() const;
    size_t getBufferIdx() const;
	char readChar();
	void readWord(char *word, size_t *word_len);
    void readUntilDelimiter(char *word, size_t *word_len, const char delimiter);
	void readWhiteSpaces();
	bool readLineDelimiter();

    /**
     * In case when there are more words on line than strings_len, fill whole
     * strings buffer and ignore rest of the line (just move the read head).
     *
     * @note line delimiter is @code \r\n @endcode
     * @param strings      ... buffer for words on line
     * @param strings_len  ... length of the @param strings buffer
     * @param max_word_len ... max length of a word in the line
     * @param word_count   ... may be nullptr
     */
    template <size_t strings_len, size_t max_word_len>
    void readLine(char (&strings)[strings_len][max_word_len], size_t *word_count)
    {
        size_t word_idx = 0;
        while (!atEndOfLine() && !atEnd()) {
            readWhiteSpaces();

            char word[max_word_len];
            size_t word_len = 0;
            readWord(word, &word_len);
            // Copy word into buffer when there is space.
            if (word_idx < strings_len) {
                std::memcpy(strings[word_idx], word, word_len);
            }
            word_idx++;
        }
        if (word_count != nullptr) {
            *word_count = word_idx;
        }
        // Read "\r\n"
        shiftBuffer(2);
    };

    /**
     * See readLine above.
     * @param delimiter ... delimiter of words on line
     */
    template <size_t strings_len, size_t max_word_len>
    void readLine(char (&strings)[strings_len][max_word_len], size_t *word_count, const char delimiter)
    {
        size_t word_idx = 0;
        while (!atEndOfLine() && !atEnd()) {
            char word[max_word_len];
            size_t word_len = 0;
            readUntilDelimiter(word, &word_len, delimiter);

            // Read delimiter
            shiftBuffer(1);

            // Copy word into buffer when there is space.
            if (word_idx < strings_len) {
                std::memcpy(strings[word_idx], word, word_len);
            }
            word_idx++;
        }
        if (word_count != nullptr) {
            *word_count = word_idx;
        }
        // Read "\r\n"
        shiftBuffer(2);
    };

	const char * getRestOfBuffer();
	bool atEnd() const;
    bool atEmptyLine() const;

private:
	const char *buffer;
	const size_t bufferSize;
	size_t bufferIdx;

    bool isWhiteSpace(char c) const;
    void unreadChar();
    bool atEndOfLine() const;
    void shiftBuffer(size_t count);
};

#endif /* CHAR_STREAM_HPP_ */
