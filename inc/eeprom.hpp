/*
 * eeprom.hpp
 *
 *  Created on: Jul 18, 2017
 *      Author: mayfa
 */

#ifndef INC_EEPROM_HPP_
#define INC_EEPROM_HPP_

#include <vector>
#include "stm3210c_eval_eeprom.h"
#include "intervalframe_data.hpp"

/**
 * Singleton class.
 */
class eeprom {
	eeprom();
	const uint32_t FRAME_DELIM = 0x1234;
	void save(const intervalframe_data &data, uint16_t addr);
	void load(intervalframe_data &data, uint16_t addr);
	uint32_t read_page(uint16_t addr);
	void write_page(uint32_t page, uint16_t addr);
public:
	static eeprom & get_instance();
	void operator=(const eeprom &) = delete;
	eeprom(const eeprom &) = delete;
	bool is_empty();
	void save(const std::vector<intervalframe_data> &data_vec);
	void load(std::vector<intervalframe_data> &data_vec);
};

#endif /* INC_EEPROM_HPP_ */
