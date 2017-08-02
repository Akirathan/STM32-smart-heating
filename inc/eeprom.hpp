/*
 * eeprom.hpp
 *
 *  Created on: Jul 18, 2017
 *      Author: mayfa
 */

#ifndef INC_EEPROM_HPP_
#define INC_EEPROM_HPP_

#include <interval_frame_data.hpp>
#include <vector>
#include "stm3210c_eval_eeprom.h"

/**
 * Singleton class.
 */
class EEPROM {
public:
	static EEPROM& getInstance();
	void operator=(const EEPROM&) = delete;
	EEPROM(const EEPROM&) = delete;
	bool isEmpty();
	void save(const std::vector<IntervalFrameData>& data_vec);
	void load(std::vector<IntervalFrameData>& data_vec);
private:
	EEPROM();
	const uint32_t FRAME_DELIM = 0x1234;
	void save(const IntervalFrameData &data, uint16_t addr);
	void load(IntervalFrameData &data, uint16_t addr);
	uint32_t readPage(uint16_t addr);
	void writePage(uint32_t page, uint16_t addr);
};

#endif /* INC_EEPROM_HPP_ */
