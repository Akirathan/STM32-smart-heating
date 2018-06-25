/**
 * @file header_option.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef HEADER_OPTION_HPP_
#define HEADER_OPTION_HPP_

#include <cstddef>

namespace http {

/**
 * @brief Represents one HTTP header option.
 *
 * @note Currently only Content-Type, Content-Length and host options are supported.
 */
class HeaderOption {
private:
    static const size_t VALUE_LEN = 50;
    static const size_t TYPE_LEN = 20;

public:
    static const size_t MAX_SIZE = TYPE_LEN + VALUE_LEN;
    static constexpr const char *CONTENT_TYPE_STR = "Content-Type";
    static constexpr const char *CONTENT_LENGTH_STR = "Content-Length";
    static constexpr const char *HOST_STR = "Host";

	enum Type {
        UNKNOWN_TYPE,
		CONTENT_TYPE,
		CONTENT_LENGTH,
		HOST
	};

	HeaderOption();
    HeaderOption(Type type, const char *value);
	void setType(Type type);
    Type getType() const;
    void setValue(const char *newValue);
    const char * getValue() const;
	size_t getTotalSize() const;
	void toBuffer(char *buffer) const;

private:
	Type type;
	char value[VALUE_LEN];

	size_t getTypeSize() const;
	size_t getValueSize() const;
};

} // namespace http

#endif /* HEADER_OPTION_HPP_ */
