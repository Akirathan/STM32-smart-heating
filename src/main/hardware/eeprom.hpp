/**
 * @file eeprom.hpp
 * @author Pavel Marek
 * @date 18.7.2017
 */

#ifndef INC_EEPROM_HPP_
#define INC_EEPROM_HPP_

#include "interval_frame_data.hpp"
#include "stm3210c_eval_eeprom.h"
#include "des_key.hpp"
#include "settings.h" // For INTERVALS_NUM

/**
 * @brief Class representing EEPROM compoment.
 *
 * This class is used for storing interval frame data into EEPROM. Data are
 * written as words (32 bits). First and last words are @ref FRAME_DELIM.
 *
 * @note Implemented as a singleton class.
 */
class EEPROM {
public:
	static EEPROM& getInstance();
	void operator=(const EEPROM&) = delete;
	EEPROM(const EEPROM&) = delete;
	void reset();
	bool isEmpty();
	bool isOperationInProgress();
	void save(const IntervalFrameData data[], const size_t count, uint32_t timestamp, bool time_synced);
	void load(IntervalFrameData data[], size_t* count, uint32_t *timestamp, bool *time_synced);
	void saveIntervalsMetadata(uint32_t timestamp, bool time_synced);
	void loadIntervalsMetadata(uint32_t *timestamp, bool *time_synced);
	bool isKeySet();
	void saveKey(const DesKey &key);
	DesKey loadKey();
private:
	const uint16_t START_ADDR = 0;

	const uint16_t KEY_SET_FLAG_ADDR = START_ADDR;
	const uint16_t KEY_START_ADDR = KEY_SET_FLAG_ADDR + 4;
	const uint16_t KEY_END_ADDR = KEY_START_ADDR + 8;

	const uint16_t INTERVALS_TIMESTAMP_ADDR = KEY_END_ADDR;
	const uint16_t INTERVALS_TIMESYNCED_ADDR = INTERVALS_TIMESTAMP_ADDR + 4;
	const uint16_t INTERVALS_NUM_ADDR = INTERVALS_TIMESYNCED_ADDR + 4;
	const uint16_t INTERVALS_START_ADDR = INTERVALS_NUM_ADDR + 4;
	const uint16_t INTERVALS_END_ADDR = INTERVALS_START_ADDR + 12 * INTERVALS_NUM;

	bool operationInProgress;

	EEPROM();

	void save(const IntervalFrameData &data, uint16_t addr);
	void load(IntervalFrameData &data, uint16_t addr);
	uint32_t readPage(uint16_t addr);
	void writePage(uint32_t page, uint16_t addr);
	void readBuffer(uint8_t *buffer, uint32_t *numbytes, uint16_t addr);
	void writeBuffer(const uint8_t *buffer, uint32_t numbytes, uint16_t addr);
};

#endif /* INC_EEPROM_HPP_ */
