/*
 * intervalframe_data.cpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#include "interval_frame_data.hpp"

IntervalFrameData::IntervalFrameData()
	: IntervalFrameData(0, 0, 0, 0, false)
{ }

IntervalFrameData::IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp)
	: IntervalFrameData(from, to, temp, 0, false)
{ }

IntervalFrameData::IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp, uint32_t timestamp, bool timestampSynchronized)
	: from(from),
	  to(to),
	  temp(temp),
	  timestamp(timestamp),
	  timestampSynchronized(timestampSynchronized)
{ }

bool IntervalFrameData::isSet() const
{
	return (from != 0) || (to != 0) || (temp != 0);
}
