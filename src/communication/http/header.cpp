/**
 * @file header.cpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#include "header.hpp"
#include "rt_assert.h"

namespace http {

Header::Header() :
		optionsIdx(0)
{}

/**
 * Copies given option into this Header and fails if there are already too many options.
 */
void Header::appendOption(const HeaderOption &option)
{
	rt_assert(optionsIdx + 1 <= MAX_OPTIONS, "Header: too much options");
	options[optionsIdx++] = option;
}

/**
 * Searches for an option by given type.
 * @return ... nullptr if no such option was found.
 */
const char *Header::getOptionValueByType(HeaderOption::Type type) const
{
	for (size_t i = 0; i < optionsIdx; ++i) {
		if (options[i].getType() == type) {
			return options[i].getValue();
		}
	}
	return nullptr;
}

void Header::setOptionValue(HeaderOption::Type type, const char *value)
{
	for (size_t i = 0; i < optionsIdx; i++) {
		if (options[i].getType() == type) {
			options[i].setValue(value);
		}
	}
}

/**
 * Returns total size (of bytes) of this header including trailing linebreak.
 */
size_t Header::getTotalSize() const
{
	size_t total_size = 0;
	for (size_t i = 0; i < optionsIdx; ++i) {
		total_size += options[i].getTotalSize();
	}
	return total_size;
}

/**
 * @brief Serializes this header into given text buffer.
 *
 * @param buffer ... after serialization is done, this buffer will contain trailing
 *                   null byte like normal C-string.
 */
void Header::toBuffer(char *buffer) const
{
    for (size_t i = 0; i < optionsIdx; i++) {
        options[i].toBuffer(buffer);
        buffer += options[i].getTotalSize();
    }
}

} // namespace http
