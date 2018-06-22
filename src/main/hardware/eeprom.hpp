/**
 * @file eeprom.hpp
 * @author Pavel Marek
 * @date 18.7.2017
 */

#ifndef INC_EEPROM_HPP_
#define INC_EEPROM_HPP_

#include "interval_frame_data.hpp"
#include "stm3210c_eval_eeprom.h"

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
	bool isEmpty();
	void save(const IntervalFrameData data[], const size_t count);
	void load(IntervalFrameData data[], size_t* count);
private:
	EEPROM();
	const uint32_t FRAME_DELIM = 0x1234;
	void save(const IntervalFrameData &data, uint16_t addr);
	void load(IntervalFrameData &data, uint16_t addr);
	uint32_t readPage(uint16_t addr);
	void writePage(uint32_t page, uint16_t addr);
};

#endif /* INC_EEPROM_HPP_ */
