//
// Created by mayfa on 14.5.18.
//

#ifndef COMMUNICATION_DEVICE_HPP
#define COMMUNICATION_DEVICE_HPP

#include "interval_list.hpp"
#include "i_client_cb_recver.hpp"

/**
 * @brief Represents this STM device in "asynchronous communication with server" context.
 *
 * This class is directly communicating with @ref Client which represents connection
 * to the server. This means that everything that is exchanged with server goes
 * through this class.
 *
 * This class should be used as follows:
 * First call @ref setTemp or @ref setIntervals to save data for exchange with
 * the server. Then call @ref connect to start the communication cycle in @ref Client.
 * "Indefinite" communication (more specifically data exchange) takes place from
 * this moment on. You may however stop this cycle with @ref disconnect method and
 * resume it again with @ref connect.
 *
 * Call @ref setTemp method to send the temperature to the server. Note that the
 * timestamp for temperature is appended in this method.
 *
 * Call @ref setIntervals method to save those intervals for exchange with the
 * server. Exchange means that timestamp of those intervals is compared with the
 * timestamp from the server's intervals and based on this comparison are either
 * send to the server or the server's intervals are downloaded.
 * @ref intervalsSentCb is a callback function that is called from @ref Client
 * when intervals were sent to the server.
 * @ref intervalsRecvCb is a callback function that is called from @ref Client
 * when intervals were downloaded from the server.
 *
 * See @ref Client for more information about communication with server.
 */
class CommunicationDevice : public IClientCbRecver {
private:
    static const size_t KEY_LEN = 9;

public:
    static const size_t ID_LEN = 15;

    CommunicationDevice();
    /***************** Getters and setters *********************/
    const char * getKey() const;
    void setKey(const char *key);
    const char * getId() const;
    bool isConnected() const;
    double getTemp() const;
    void setTemp(double temp);
    void setIntervals(const IntervalList &interval_list);
    const IntervalList & getIntervals() const;
    /***********************************************************/
    /*************** Callbacks from client *********************/
    void connectedCb() override;
    void tempSentCb() override;
    void intervalsSentCb() override;
    void intervalsRecvCb(const IntervalList &interval_list) override;
    /***********************************************************/
    bool connect();
    void disconnect();
    void synchronizeTime(uint32_t server_real_time);
    bool isTimeSynchronized() const;
    uint32_t getCurrentTimestamp() const;

private:
    char key[KEY_LEN];
    char id[ID_LEN];
    double temp;
    uint32_t tempTimestamp;
    IntervalList intervalList;
    bool connected;
};


#endif //COMMUNICATION_DEVICE_HPP
