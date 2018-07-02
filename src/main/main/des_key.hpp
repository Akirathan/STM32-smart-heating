/**
 * @file des_key.hpp
 * @author Pavel Marek
 * @date Jul 2, 2018
 */

#ifndef DES_KEY_HPP_
#define DES_KEY_HPP_

#include <cstdint>
#include <cstddef> // For size_t

class DesKey {
public:
	static const size_t SIZE = 8;

	static DesKey deserialize(const uint8_t *buff);

	DesKey(const char *hex_str);
	void serialize(uint8_t *buff);
private:
	uint8_t content[SIZE];

	DesKey();
	uint8_t hexCharToByte(const char *hex_byte);
	uint8_t convertHexChar(char hex_char);
};

#endif /* DES_KEY_HPP_ */
