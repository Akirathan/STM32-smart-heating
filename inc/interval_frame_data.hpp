/*
 * intervalframe_data.hpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#ifndef INTERVAL_FRAME_DATA_HPP_
#define INTERVAL_FRAME_DATA_HPP_

#include "stm3210c_eval.h"

struct IntervalFrameData {
	IntervalFrameData();
	IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp);
	uint32_t from;
	uint32_t to;
	uint32_t temp;
};

#endif /* INTERVAL_FRAME_DATA_HPP_ */
