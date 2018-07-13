//
// Created by mayfa on 14.5.18.
//

#include "communication_device.hpp"
#include <cstring>
#include "client.hpp"
#include "rt_assert.h"
#include "rtc_controller.hpp"
#include "connected_event.hpp"
#include "intervals_changed_server_event.hpp"
#include "application.hpp"
#include "des.hpp"

/**
 * Device ID is generated from UID.
 */
CommunicationDevice::CommunicationDevice() :
		keySet(false),
        temp(0.0),
        tempTimestamp(0),
        connected(false),
		timeSynced(false)
{
    std::strcpy(id, "stm1");
}

/**
 * Returns 8-byte long DES key.
 */
const DesKey & CommunicationDevice::getKey() const
{
    return key;
}

/**
 * Sets the DES key into this CommunicationDevice.
 * @param key ... DES key 8-bytes long
 */
void CommunicationDevice::setKey(const DesKey &key)
{
	this->key = key;
	DES::init(key.getContent());
	keySet = true;
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
 *
 * Temperature is saved for sending to the server only in case when time is
 * already synchronized. Otherwise timestamp that would be sent to the server
 * would be incorrect.
 *
 * @param temp ... temperature to set.
 */
void CommunicationDevice::setTemp(double temp)
{
	if (timeSynced) {
		this->temp = temp;
		tempTimestamp = getCurrentTimestamp();

		Client::setTemperature(temp, tempTimestamp);
	}
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
	if (timeSynced) {
		intervalList = interval_list;
		intervalList.setTimestamp(getCurrentTimestamp());

		Client::setIntervals(intervalList);
	}
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
 *
 * @param server_real_time ... timestamp sent from server for synchronization.
 */
void CommunicationDevice::connectedCb(uint32_t server_real_time)
{
	ConnectedEvent connectedEvent(getCurrentTimestamp(), server_real_time);
	Application::emitEvent(connectedEvent);

    connected = true;
	timeSynced = true;
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
	rt_assert(interval_list.getTimestamp() != 0, "Timestamp must be set");

	IntervalsChangedServerEvent event(interval_list, timeSynced);
	Application::emitEvent(event);

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
	rt_assert(keySet, "Key must be set");
	rt_assert(std::strlen(id) != 0, "Id must be set");

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
    timeSynced = false;
}

/**
 * @brief Returns true if time was synchronized with server.
 *
 * @note Time synchronization takes place during connecting to the server.
 */
bool CommunicationDevice::isTimeSynchronized() const
{
	return timeSynced;
}

/**
 * @brief Gets current (Unix) timestamp.
 *
 *
 * @return
 */
uint32_t CommunicationDevice::getCurrentTimestamp() const
{
	return RTCController::getInstance().getTimestamp();
}


