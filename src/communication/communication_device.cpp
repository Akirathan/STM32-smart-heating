//
// Created by mayfa on 14.5.18.
//

#include "communication_device.hpp"
#include <cstring>
#include "client.hpp"

CommunicationDevice::CommunicationDevice() :
        temp(0.0),
        tempTimestamp(0),
        connected(false)
{
    std::strcpy(this->id, id);
    std::strcpy(this->key, key);
}

const char *CommunicationDevice::getKey() const
{
    return key;
}

/**
 * Returns this Device ID.
 * Note that this ID is "generated" in constructor from STM's UID.
 * @return
 */
const char *CommunicationDevice::getId() const
{
    return id;
}

/**
 * Returns bool whether this STM is connected to the server.
 */
bool CommunicationDevice::isConnected() const
{
    return connected;
}

double CommunicationDevice::getTemp() const
{
    return temp;
}

/**
 * Sets the device temperature. Note that this temperature is periodically sent
 * to the server if this device is connected.
 * @param temp ... temperature to set.
 */
void CommunicationDevice::setTemp(double temp)
{
    this->temp = temp;
    tempTimestamp = getCurrentTimestamp();

    Client::setTemperature(temp, tempTimestamp);
}

/**
 * @brief Saves the intervals for later exchange with server.
 *
 * Server's timestamp and this timestamp are compare in that exchange and based on
 * that comparison intervals are either sent to the server (and @ref intervalsSentCb
 * is called) or downloaded from the server (and @ref intervalsRecvCb is called).
 *
 * @param interval_list ... intervals to be set, timestamp of this interval_list has
 *                          to be set.
 */
void CommunicationDevice::setIntervals(const IntervalList &interval_list)
{
    intervalList = interval_list;
    intervalList.setTimestamp(getCurrentTimestamp());

    Client::setIntervals(intervalList);
}

/**
 * @brief Returns the @ref IntervalList that was exchanged with the server.
 *
 * Returned intervals are either those that were set with the @ref setIntervals
 * method, or those that were downloaded from the server. Depends which timestamp
 * was higher.
 */
const IntervalList & CommunicationDevice::getIntervals() const
{
    return intervalList;
}

/**
 * @brief Callback frunction called from @ref Client when it connects to the server.
 */
void CommunicationDevice::connectedCb()
{

}

/**
 * @brief Callback function called from @ref Client when temperature was sent to the server.
 */
void CommunicationDevice::tempSentCb()
{

}

/**
 * @brief Callback function called from @ref Client when intervals were sent to the server.
 *
 * This means that the intervals that were saved into this CommunicationDevice
 * with @ref setIntervals method, have higher timestamp (they are newer) than
 * the timestamp from server's intervals.
 */
void CommunicationDevice::intervalsSentCb()
{

}

/**
 * @brief Callbak function called from @ref Client when intervals were received from the server.
 *
 * Intervals are received from and currently set intervals are "silently"
 * overwriten.
 *
 * @param interval_list ... received intervals from the server.
 */
void CommunicationDevice::intervalsRecvCb(const IntervalList &interval_list)
{
    intervalList = interval_list;
}

/**
 * @brief Sends connect request to server.
 *
 * Note that this connect request starts the communication cycle in @ref Client.
 *
 * @return bool flag whether the connection was succesfull.
 */
bool CommunicationDevice::connect()
{
    connected = true;
    return Client::sendConnectReq(id);
}

/**
 * @brief Disconnects from the server.
 *
 * The data exchange (communication cycle) is stopped. It may be started again
 * with @ref connect function.
 */
void CommunicationDevice::disconnect()
{
    connected = false;
    Client::disconnect();
}

/**
 * @brief Gets current (Unix) timestamp.
 *
 *
 * @return
 */
uint32_t CommunicationDevice::getCurrentTimestamp() const
{

}


