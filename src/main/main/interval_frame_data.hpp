/*
 * intervalframe_data.hpp
 *
 *  Created on: Jul 26, 2017
 *      Author: mayfa
 */

#ifndef INTERVAL_FRAME_DATA_HPP_
#define INTERVAL_FRAME_DATA_HPP_

#include <cstdint>

/**
 * Represents one interval configuration data that is saved into EEPROM.
 * Note that for for sending to server, @ref Interval is used.
 * TODO: merge IntervalFrameData and Interval into one class.
 */
struct IntervalFrameData {
	IntervalFrameData();
	IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp);
	IntervalFrameData(uint32_t from, uint32_t to, uint32_t temp, uint32_t timestamp, bool timestampSynchronized);
	bool isSet() const;
	uint32_t from;
	uint32_t to;
	uint32_t temp;
	uint32_t timestamp;
	bool timestampSynchronized;
};

#endif /* INTERVAL_FRAME_DATA_HPP_ */
