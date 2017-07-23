/*
 * eeprom.cpp
 *
 *  Created on: Jul 18, 2017
 *      Author: mayfa
 */

#include "eeprom.hpp"

eeprom & eeprom::get_instance()
{
	static eeprom instance;
	return instance;
}

eeprom::eeprom()
{
	BSP_EEPROM_SelectDevice(BSP_EEPROM_M24C64_32);

	if (BSP_EEPROM_Init() != EEPROM_OK) {
		//error_handler(); TODO:
	}
}

void eeprom::save(const intervalframe_data &data, uint16_t addr)
{
	this->write_page(data.from, addr);
	addr += 4;
	this->write_page(data.to, addr);
	addr += 4;
	this->write_page(data.temp, addr);
}

void eeprom::load(intervalframe_data &data, uint16_t addr)
{
	data.from = this->read_page(addr);
	addr += 4;
	data.to = this->read_page(addr);
	addr += 4;
	data.temp = this->read_page(addr);
}

void eeprom::write_page(uint32_t page, uint16_t addr)
{
	uint8_t buff[4] = {0};

	for (int i = 3; i >= 0; --i) {
		buff[i] |= page;
		if (i != 0) page >>= 8;
	}

	BSP_EEPROM_WriteBuffer(buff, addr, 4);
}

// TODO: error handling
uint32_t eeprom::read_page(uint16_t addr)
{
	uint8_t buff[4];
	uint32_t num_bytes = 4;
	uint32_t word = 0;

	BSP_EEPROM_ReadBuffer(buff, addr, &num_bytes);

	for (int i = 0; i < 4; ++i){
		word |= buff[i];
		if (i != 3) {
			word <<= 8;
		}
	}
	return word;
}

void eeprom::save(const std::vector<intervalframe_data>& data_vec)
{
	uint16_t addr = 0;

	/* Write starting delimiter */
	this->write_page(this->FRAME_DELIM, addr);
	addr += 4;

	for (const intervalframe_data &data : data_vec) {
		this->save(data, addr);
		addr += sizeof(data);
	}

	// Write ending delimiter
	this->write_page(this->FRAME_DELIM, addr);
}

bool eeprom::is_empty()
{
	if (this->read_page(0) != this->FRAME_DELIM) {
		return true;
	}
	else {
		return false;
	}
}

// TODO: error handling
void eeprom::load(std::vector<intervalframe_data>& data_vec)
{
	uint16_t addr = 0;

	if (this->read_page(addr) != this->FRAME_DELIM) {
		// Error: expected start of frame
		return;
	}

	addr += 4;
	while (this->read_page(addr) != this->FRAME_DELIM) {
		intervalframe_data data;

		this->load(data, addr);
		data_vec.push_back(data);
		addr += sizeof(data);
	}
}


