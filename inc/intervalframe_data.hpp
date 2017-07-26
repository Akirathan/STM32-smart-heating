/*
 * intervalframe_data.hpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#ifndef INTERVALFRAME_DATA_HPP_
#define INTERVALFRAME_DATA_HPP_

#include "stm3210c_eval.h"

struct intervalframe_data {
	intervalframe_data();
	intervalframe_data(uint32_t from, uint32_t to, uint32_t temp);
	uint32_t from;
	uint32_t to;
	uint32_t temp;
};

#endif /* INTERVALFRAME_DATA_HPP_ */
