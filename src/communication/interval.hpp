//
// Created by mayfa on 14.5.18.
//

#ifndef DEVICE_SIMULATOR_INTERVAL_HPP
#define DEVICE_SIMULATOR_INTERVAL_HPP

#include <cstdint>

class Interval {
public:
    static const size_t SIZE = 12;

    static Interval deserialize(const uint8_t *buffer, const size_t buff_size);

    Interval(const uint32_t fromTime, const uint32_t toTime, const uint32_t temp);
    Interval();
    bool isSet() const;
    void serialize(uint8_t *buffer) const;
    void setFromTime(const uint32_t fromTime);
    void setToTime(const uint32_t toTime);
    void setTemp(const uint32_t temp);
    uint32_t getFromTime() const;
    uint32_t getToTime() const;
    uint32_t getTemp() const;

private:
    uint32_t fromTime;
    uint32_t toTime;
    uint32_t temp;
};

#endif //DEVICE_SIMULATOR_INTERVAL_HPP
