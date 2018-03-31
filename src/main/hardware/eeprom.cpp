/**
 * @file eeprom.cpp
 * @author Pavel Marek
 * @date 18.7.2017
 */

#include "eeprom.hpp"

EEPROM& EEPROM::getInstance()
{
	static EEPROM instance;
	return instance;
}

EEPROM::EEPROM()
{
	BSP_EEPROM_SelectDevice(BSP_EEPROM_M24C64_32);

	uint32_t ret = BSP_EEPROM_Init();
	rt_assert(ret == EEPROM_OK, "BSP EEPROM initialization failed");
}

void EEPROM::save(const IntervalFrameData& data, uint16_t addr)
{
	writePage(data.from, addr);
	addr += 4;
	writePage(data.to, addr);
	addr += 4;
	writePage(data.temp, addr);
}

void EEPROM::load(IntervalFrameData& data, uint16_t addr)
{
	data.from = readPage(addr);
	addr += 4;
	data.to = readPage(addr);
	addr += 4;
	data.temp = readPage(addr);
}

void EEPROM::writePage(uint32_t page, uint16_t addr)
{
	uint8_t buff[4] = {0};

	for (int i = 3; i >= 0; --i) {
		buff[i] |= page;
		if (i != 0) page >>= 8;
	}

	uint32_t error_code = BSP_EEPROM_WriteBuffer(buff, addr, 4);
	rt_assert(error_code == EEPROM_OK, "BSP EEPROM Write buffer failed");
}

uint32_t EEPROM::readPage(uint16_t addr)
{
	uint8_t buff[4];
	uint32_t num_bytes = 4;
	uint32_t word = 0;

	uint32_t error_code = BSP_EEPROM_ReadBuffer(buff, addr, &num_bytes);
	rt_assert(error_code == EEPROM_OK, "BSP EEPROM read buffer failed");

	for (int i = 0; i < 4; ++i){
		word |= buff[i];
		if (i != 3) {
			word <<= 8;
		}
	}
	return word;
}

void EEPROM::save(const std::vector<IntervalFrameData>& data_vec)
{
	uint16_t addr = 0;

	// Write starting delimiter.
	writePage(FRAME_DELIM, addr);
	addr += 4;

	for (const IntervalFrameData& data : data_vec) {
		save(data, addr);
		addr += sizeof(data);
	}

	// Write ending delimiter
	writePage(FRAME_DELIM, addr);
}

bool EEPROM::isEmpty()
{
	if (readPage(0) != FRAME_DELIM) {
		return true;
	}
	else {
		return false;
	}
}

/**
 * @brief Loads data stored in EEPROM into @p data_vec.
 *
 * If EEPROM is empty, no data is pushed to @p data_vec.
 */
void EEPROM::load(std::vector<IntervalFrameData>& data_vec)
{
	uint16_t addr = 0;

	if (readPage(addr) != FRAME_DELIM) {
		return;
	}

	addr += 4;
	while (readPage(addr) != FRAME_DELIM) {
		IntervalFrameData data;

		load(data, addr);
		data_vec.push_back(data);
		addr += sizeof(data);
	}
}
