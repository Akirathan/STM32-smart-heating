//
// Created by mayfa on 14.5.18.
//

#include <cstddef>
#include "interval.hpp"
#include "rt_assert.h"

Interval Interval::deserialize(const uint8_t *buffer, const size_t buff_size)
{
    rt_assert(buff_size == SIZE, "Interval: wrong size of buffer");

    uint32_t from_time = *((const uint32_t *) buffer);
    buffer += 4;
    uint32_t to_time = *((const uint32_t *) buffer);
    buffer += 4;
    uint32_t temp = *((const uint32_t *) buffer);
    return Interval(from_time, to_time, temp);
}

/**
 * @param fromTime ... serialized time (hours*60 + minutes)
 * @param toTime   ... serialized time (hours*60 + minutes)
 * @param temp     ... temperature in this interval.
 */
Interval::Interval(const uint32_t fromTime, const uint32_t toTime, const uint32_t temp) :
        fromTime(fromTime),
        toTime(toTime),
        temp(temp)
{}

Interval::Interval() :
        Interval(0, 0, 0)
{}

bool Interval::isSet() const
{
	return fromTime != 0 && toTime != 0 && temp != 0;
}

void Interval::serialize(uint8_t *buffer) const
{
    *((uint32_t *) buffer) = fromTime;
    buffer += 4;
    *((uint32_t *) buffer) = toTime;
    buffer += 4;
    *((uint32_t *) buffer) = temp;
}

void Interval::setFromTime(const uint32_t fromTime)
{
    this->fromTime = fromTime;
}

void Interval::setToTime(const uint32_t toTime)
{
    this->toTime = toTime;
}

void Interval::setTemp(const uint32_t temp)
{
    this->temp = temp;
}

uint32_t Interval::getFromTime() const
{
    return fromTime;
}

uint32_t Interval::getToTime() const
{
    return toTime;
}

uint32_t Interval::getTemp() const
{
    return temp;
}
