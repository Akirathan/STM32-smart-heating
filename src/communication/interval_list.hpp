//
// Created by mayfa on 14.5.18.
//

#ifndef DEVICE_SIMULATOR_INTERVAL_LIST_HPP
#define DEVICE_SIMULATOR_INTERVAL_LIST_HPP

#include <cstddef>
#include <cstdint>
#include "interval.hpp"

class IntervalList {
public:
    static const size_t MAX_NUM = 20;
    static const size_t MAX_SIZE = MAX_NUM * Interval::SIZE;

    static IntervalList deserialize(const uint8_t *buffer, const size_t buff_size);

    IntervalList();
    void setTimestamp(uint32_t time_stamp);
    uint32_t getTimestamp() const;
    void addInterval(const Interval &interval);
    const Interval * getInterval(size_t index) const;
    size_t getIntervalsCount() const;
    bool isEmpty() const;
    void serialize(uint8_t *buffer, size_t *buff_len) const;

private:
    Interval intervals[MAX_NUM];
    size_t intervalsIdx;
    uint32_t timeStamp;
};

#endif //DEVICE_SIMULATOR_INTERVAL_LIST_HPP
