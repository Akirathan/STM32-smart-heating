/**
 * @file client.hpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <cstdint>
#include "http/request.hpp"
#include "http/response.hpp"
#include "communication_device.hpp"
#include "interval_list.hpp"
#include "i_client_cb_recver.hpp"

/**
 * @brief Represents application specific HTTP client (implemented as static class).
 *
 * Once this Client connects to the server, indefinite data exchange cycle starts.
 * The cycle by itself is simple, it goes:
 *   GET intervals/timestamp --> POST intervals or GET intervals --> POST temperature
 *
 * Implementation is however not so simple, mainly because LwIP without use of
 * RTOS allows us only to use its callback API. Therefore we need to implement
 * some kind of state automaton. See documentation resources for pictures and
 * other text about the implementation.
 *
 * Usage:
 * First you have to call @ref init before any other method.
 * Similarly to @ref CommunicationDevice you may now call either some set method
 * (more specificaly @ref setTemperature or @ref setIntervals) or @ref startCycle
 * or @ref sendConnectReq.
 * Note that @ref sendConnectReq and @ref startCycle are more or less equivalents
 * but it is encouraged to use @ref sendConnectReq when first using this class.
 *
 * @see There are more documentation resources (mostly in Czech language) concerning
 * the implementation and specifics of this class.
*/
class Client {
private:
    static const size_t IP_ADDR_LEN = 16;
    static const size_t HOST_LEN = IP_ADDR_LEN + 7;

    enum await_state_t {
        AWAIT_NONE,
        AWAIT_CONNECT_RESPONSE,
        AWAIT_INTERVAL_TIMESTAMP_RESPONSE,
        AWAIT_INTERVALS,
        AWAIT_INTERVALS_ACK,
        AWAIT_TEMP_ACK
    };

public:
    static void init(const char *host, uint16_t port, IClientCbRecver *client_cb_recver);
    static void receiveCb(http::Response &response);
    static bool sendConnectReq(const char *device_id);
    static void startCycle();
    static void disconnect();
    static bool isConnected();
    static void setTemperature(const double temp, const uint32_t time_stamp);
    static void setIntervals(const IntervalList &intervals);

private:
    static const constexpr char * CONNECT_URL = "/controllers/connect";
    static const constexpr char * TEMP_URL = "/controllers/actual/temp";
    static const constexpr char * INTERVALS_URL = "/controllers/config/intervals";
    static const constexpr char * INTERVALS_TIMESTAMP_URL = "/controllers/config/intervals/timestamp";

    static bool initialized;
    static bool connected;
    static uint16_t port;
    static char host[HOST_LEN];
    static await_state_t state;
    static char deviceId[CommunicationDevice::ID_LEN];
    /// Preset temperature by device that will be posted to server when appropriate.
    static double temperature;
    static uint32_t temperatureTimestamp;
    /// Interval list set by device that will be posted to server when appropriate.
    static IntervalList intervalList;
    /// Temporary storage for intervals timestamp from server that is read in
    /// readIntervalTimestampResp. Note that this member is necessary because
    /// server does not send timestamp with intervals in one response.
    static uint32_t tempIntervalsTimestamp;
    static IClientCbRecver *clientCbRecver;

    static void initHost(const char *host, const uint16_t port);
    static bool send(http::Request request, bool await_body);
    static void readConnectResponse(const http::Response &response);
    static void readIntervalTimestampResp(const http::Response &response);
    static void readIntervalsResp(const http::Response &response, const uint32_t time_stamp);
    static void readIntervalsAckResp(const http::Response &response);
    static void readTempAckResp(const http::Response &response);
    static http::Request createConnectReq(const char *device_id);
    static http::Request createIntervalTimestampReq();
    static http::Request createGetIntervalsReq();
    static http::Request createPostIntervalsReq(const IntervalList &interval_list);
    static http::Request createPostTemperature(const double temp, const uint32_t time_stamp);
    static http::Request createGetReq(const char *url);
    static http::Request createPostReq(const char *url, const char *body, const size_t body_len,
                                       const char *content_type);
    static void callConnectedCb(uint32_t server_real_time);
    static void callTempSentCb();
    static void callIntervalsSentCb();
    static void callIntervalsRecvCb(const IntervalList &interval_list);
};



#endif /* CLIENT_HPP_ */
