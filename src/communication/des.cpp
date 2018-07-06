/**
 * @file des.cpp
 * @author Pavel Marek
 * @date Jul 4, 2018
 */

#include "des.hpp"
#include "crypto.h"
#include "rt_assert.h"

uint8_t DES::key[KEY_SIZE] = {0};
bool    DES::keySet = false;
uint8_t DES::buffer[MAX_BUFFER_SIZE] = {0};
size_t  DES::bufferIdx = 0;

void DES::init(const uint8_t *key)
{
	for (size_t i = 0; i < KEY_SIZE; i++) {
		DES::key[i] = key[i];
	}
	keySet = true;
}

/**
 * If input buffer's size is not multiple of 8, rest of the buffer is filled with
 * null bytes and then encrypted.
 * @param in_buff
 * @param in_len
 * @param out_buff
 * @param out_size
 * @return
 */
bool DES::encrypt(const uint8_t *in_buff, const int32_t in_len, uint8_t *out_buff, int32_t *out_size)
{
	rt_assert(keySet, "Key must be set before encryption");

	resetInnerBuffer();
	paddToInnerBuffer(in_buff, in_len);

	DESECBctx_stt des_ctx;
	des_ctx.mFlags = E_SK_DEFAULT;

	int32_t state = DES_ECB_Encrypt_Init(&des_ctx, key, nullptr);
	rt_assert(state == DES_SUCCESS, "DES init failed");

	int32_t tmp_out_len = 0;
	state = DES_ECB_Encrypt_Append(&des_ctx, buffer, bufferIdx, out_buff, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "DES encrypt append failed");

	*out_size = tmp_out_len;
	return true;
}

bool DES::decrypt(const uint8_t *in_buff, const int32_t in_len, uint8_t *out_buff, int32_t *out_size)
{
	rt_assert(keySet, "Key must be set before decryption");

	resetInnerBuffer();
	paddToInnerBuffer(in_buff, in_len);

	DESECBctx_stt des_ctx;
	des_ctx.mFlags = E_SK_DEFAULT;

	int32_t state = DES_ECB_Decrypt_Init(&des_ctx, key, nullptr);
	rt_assert(state == DES_SUCCESS, "DES_ECB_Decrypt_init failed");

	int32_t tmp_out_len = 0;
	state = DES_ECB_Decrypt_Append(&des_ctx, buffer, bufferIdx, out_buff, &tmp_out_len);
	rt_assert(state == DES_SUCCESS, "DES_ECB_Decrypt_Append failed");

	*out_size = tmp_out_len;
	return true;
}

void DES::resetInnerBuffer()
{
	for (size_t i = 0; i < MAX_BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	bufferIdx = 0;
}

/**
 * Copies data from @p input_buff into buffer member and appends zero-byte
 * padding if necessary.
 * @param input_buff
 * @param input_buff_len
 */
void DES::paddToInnerBuffer(const uint8_t *input_buff, const size_t input_buff_len)
{
	for (size_t i = 0; i < input_buff_len; i++) {
		buffer[i] = input_buff[i];
		bufferIdx++;
	}

	if ((input_buff_len % BLOCK_SIZE) != 0) {
		while ((bufferIdx % BLOCK_SIZE) != 0) {
			buffer[bufferIdx] = 0;
			bufferIdx++;
		}
	}
}

