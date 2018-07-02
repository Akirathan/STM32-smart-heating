/**
 * @file des_key.cpp
 * @author Pavel Marek
 * @date Jul 2, 2018
 */

#include "des_key.hpp"
#include <cstring> // For std::strlen
#include "rt_assert.h"

/**
 * Deserializes DesKey from given buffer.
 * @param buff ... length of this buffer must be at least DesKey::SIZE.
 * @return
 */
DesKey DesKey::deserialize(const uint8_t *buff)
{
	DesKey des_key;
	for (size_t i = 0; i < SIZE; i++) {
		des_key.content[i] = buff[i];
	}
	return des_key;
}

/**
 * Creates DES key from a string that represents 16-long hexa string.
 * @param hex_str ... 16-length hexa string
 */
DesKey::DesKey(const char *hex_str)
{
	rt_assert(std::strlen(hex_str) == 16, "Wrong size of hexa string");

	size_t content_idx = 0;
	for (size_t i = 0; i < std::strlen(hex_str); i += 2) {
		content[content_idx] = hexCharToByte(hex_str + i);
		content_idx++;
	}
}

void DesKey::serialize(uint8_t *buff)
{
	for (size_t i = 0; i < SIZE; i++) {
		buff[i] = content[i];
	}
}

DesKey::DesKey()
{
	for (size_t i = 0; i < SIZE; i++) {
		content[i] = 0;
	}
}

/**
 * @param hex_byte ... two chars representing one byte
 * @return
 */
uint8_t DesKey::hexCharToByte(const char *hex_byte)
{
	uint8_t byte = 0;
	byte |= convertHexChar(hex_byte[0]) << 4;
	byte |= convertHexChar(hex_byte[1]);
	return byte;
}

uint8_t DesKey::convertHexChar(char hex_char)
{
	if (hex_char == '0') {
		return 0;
	}
	else if (hex_char == '1') {
		return 1;
	}
	else if (hex_char == '2') {
		return 2;
	}
	else if (hex_char == '3') {
		return 3;
	}
	else if (hex_char == '4') {
		return 4;
	}
	else if (hex_char == '5') {
		return 5;
	}
	else if (hex_char == '6') {
		return 6;
	}
	else if (hex_char == '7') {
		return 7;
	}
	else if (hex_char == '8') {
		return 8;
	}
	else if (hex_char == '9') {
		return 9;
	}
	else if (hex_char == 'a' || hex_char == 'A') {
		return 10;
	}
	else if (hex_char == 'b' || hex_char == 'B') {
		return 11;
	}
	else if (hex_char == 'c' || hex_char == 'C') {
		return 12;
	}
	else if (hex_char == 'd' || hex_char == 'D') {
		return 13;
	}
	else if (hex_char == 'e' || hex_char == 'E') {
		return 14;
	}
	else if (hex_char == 'f' || hex_char == 'F') {
		return 15;
	}

	return 0;
}

