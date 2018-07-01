/**
 * @file eeprom.cpp
 * @author Pavel Marek
 * @date 18.7.2017
 */

#include "eeprom.hpp"
#include "settings.h"
#include "rt_assert.h"

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

/**
 * @brief Saves array of data into EEPROM along with timestamp and time_synced flag.
 *
 * @param data        ... Array of data with INTERVALS_NUM length, may contain nullptr.
 * @param count       ... Number of data to save.
 * @param timestamp   ... Current (Unix) timestamp.
 * @param time_synced ... flag denoting whether time is synchronized with server.
 */
void EEPROM::save(const IntervalFrameData data[], const size_t count, uint32_t timestamp, bool time_synced)
{
	rt_assert(count <= INTERVALS_NUM, "Attempting to save too much data into EEPROM");

	uint16_t addr = 0;

	// Write starting delimiter.
	writePage(FRAME_DELIM, addr);
	addr += 4;

	writePage(timestamp, addr);
	addr += 4;
	writePage(static_cast<uint32_t>(time_synced), addr);
	addr += 4;

	for (size_t i = 0; i < count; ++i) {
		save(data[i], addr);
		addr += sizeof(data[i]);
	}

	// Write ending delimiter
	writePage(FRAME_DELIM, addr);
}

void EEPROM::saveIntervalsMetadata(uint32_t timestamp, bool time_synced)
{
	writePage(timestamp, INTERVALS_TIMESTAMP_ADDR);
	writePage(static_cast<uint32_t>(time_synced), INTERVALS_TIMESYNCED_ADDR);
}

void EEPROM::loadIntervalsMetadata(uint32_t *timestamp, bool *time_synced)
{
	if (timestamp != nullptr) {
		*timestamp = readPage(INTERVALS_TIMESTAMP_ADDR);
	}
	if (time_synced != nullptr) {
		*time_synced = static_cast<bool>(readPage(INTERVALS_TIMESYNCED_ADDR));
	}
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
 * Loads data stored in EEPROM into @p data_array, along with their
 * actual count into @p count and also timestamp and time_synced flag.
 *
 * @param data_array  ... Array of data max INTERVAL_NUMS length.
 * @param count       ... Actual number of interval data contained in EEPROM
 * @param timestamp   ... timestamp of saved intervals. May be nullptr.
 * @param time_synced ... flag denoting whether time was synchronized with the
 *                        server when intervals were saved. May be nullptr.
 *
 */
void EEPROM::load(IntervalFrameData data_array[], size_t* count, uint32_t *timestamp, bool *time_synced)
{
	uint16_t addr = 0;

	if (readPage(addr) != FRAME_DELIM) {
		return;
	}
	addr += 4;

	uint32_t _timestamp = readPage(addr);
	addr += 4;
	bool _time_synced = static_cast<bool>(readPage(addr));
	addr += 4;

	if (timestamp != nullptr) {
		*timestamp = _timestamp;
	}
	if (time_synced != nullptr) {
		*time_synced = _time_synced;
	}

	size_t data_idx = 0;
	while (readPage(addr) != FRAME_DELIM) {
		rt_assert(data_idx < INTERVALS_NUM, "There is too much intervals data in EEPROM");
		IntervalFrameData data;

		load(data, addr);
		data_array[data_idx] = data;

		data_idx++;
		addr += sizeof(data);
	}

	*count = data_idx;
}
