/*
 * intervalframe_data.cpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#include "intervalframe_data.hpp"

intervalframe_data::intervalframe_data() :
	from{0},
	to{0},
	temp{0}
{ }

intervalframe_data::intervalframe_data(uint32_t from, uint32_t to, uint32_t temp) :
	from{from},
	to{to},
	temp{temp}
{ }
