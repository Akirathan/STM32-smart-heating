/*
 * intervalframe_data.cpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#include <interval_frame_data.hpp>

IntervalFrameData::IntervalFrameData()
	: from(0), to(0), temp(0)
{ }

IntervalFrameData::IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp)
	: from(from), to(to), temp(temp)
{ }
