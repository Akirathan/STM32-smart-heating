/**
 * @file des.hpp
 * @author Pavel Marek
 * @date Jul 4, 2018
 */

#ifndef DES_HPP_
#define DES_HPP_

#include <cstdint>
#include <cstddef> // For size_t

/**
 * DES enryption/decryption static class.
 *
 * When input buffer for enrypt or decrypt is not multiple of 8 long, then
 * zero-byte padding is done first.
 * Note that zero-byte padding may be used for every message that is sent
 * to/from server.
 */
class DES {
private:
	static const size_t KEY_SIZE = 8;
	static const size_t BLOCK_SIZE = KEY_SIZE;
	static const size_t MAX_BLOCKS = 20;
public:
	static const size_t MAX_BUFFER_SIZE = MAX_BLOCKS * BLOCK_SIZE;

	static void init(const uint8_t *key);
	static bool encrypt(const uint8_t *in_buff, const int32_t in_len, uint8_t *out_buff, int32_t *out_size);
	static bool decrypt(const uint8_t *in_buff, const int32_t in_len, uint8_t *out_buff, int32_t *out_size);
private:
	static uint8_t key[KEY_SIZE];
	static bool keySet;
	static uint8_t buffer[MAX_BUFFER_SIZE];
	static size_t bufferIdx;

	static void resetInnerBuffer();
	static void paddToInnerBuffer(const uint8_t *input_buff, const size_t input_buff_len);
};


#endif /* DES_HPP_ */
