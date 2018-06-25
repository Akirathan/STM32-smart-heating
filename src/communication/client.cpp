/**
 * @file client.cpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#include "rt_assert.h"
#include "client.hpp"
#include <cstring>
#include "http/response_buffer.hpp"
#include "tcp_driver.hpp"
//#include "client_timer.hpp"

bool                  Client::initialized = false;
bool                  Client::connected = false;
char                  Client::ipAddr[IP_ADDR_LEN];
uint16_t              Client::port = 0;
char                  Client::host[HOST_LEN];
Client::await_state_t Client::state = AWAIT_NONE;
char                  Client::deviceId[CommunicationDevice::ID_LEN];
double                Client::temperature = 0.0;
uint32_t              Client::temperatureTimestamp = 0;
IntervalList          Client::intervalList;
uint32_t              Client::tempIntervalsTimestamp = 0;
IClientCbRecver *     Client::clientCbRecver = nullptr;

/**
 * @brief Initializes this Client class.
 * @param ip_addr
 * @param port
 * @param client_cb_recver ... receiver of all client callbacks, may be NULL.
 */
void Client::init(const char *ip_addr, uint16_t port, IClientCbRecver *client_cb_recver)
{
    if (!initialized) {
        std::strcpy(ipAddr, ip_addr);
        Client::port = port;
        initHost(ip_addr, port);
        clientCbRecver = client_cb_recver;
        initialized = true;
    }
}

void Client::receiveCb(const http::Response &response)
{
    rt_assert(connected, "Client must be connected before receiving anything");

    switch (state) {
        case AWAIT_CONNECT_RESPONSE:
            readConnectResponse(response);
            break;
        case AWAIT_INTERVAL_TIMESTAMP_RESPONSE:
            readIntervalTimestampResp(response);
            break;
        case AWAIT_INTERVALS:
            readIntervalsResp(response, tempIntervalsTimestamp);
            break;
        case AWAIT_INTERVALS_ACK:
            readIntervalsAckResp(response);
            break;
        case AWAIT_TEMP_ACK:
            readTempAckResp(response);
            break;
        case AWAIT_NONE:break;
    }
}

/**
 * Sends connection request to the server.
 *
 * @param device_id ... device id (TODO: encrypted?)
 * @return false when TCPDriver fails to send data.
 */
bool Client::sendConnectReq(const char *device_id)
{
    std::strcpy(deviceId, device_id);
    connected = true;
    state = AWAIT_CONNECT_RESPONSE;

    return send(createConnectReq(device_id), true);
}

/**
 * @brief Starts whole communication cycle with the server.
 *
 * Called from @ref ClientTimer.
 *
 * @note  @ref ClientTimer is mandatory so every cycle does not start immediately
 * after previous one finishes.
 */
void Client::startCycle()
{
    send(createIntervalTimestampReq(), true);
    state = AWAIT_INTERVAL_TIMESTAMP_RESPONSE;
    callTempSentCb();
}

void Client::disconnect()
{
    connected = false;
}

bool Client::isConnected()
{
    return connected;
}

/**
 * Uploads temperature to the server via POST request.
 * Note that the sending is not done immediately after calling this method.
 * @param temp        ... temperature to be uploaded to the server
 * @param time_stamp  ... timestamp of the temperature
 */
void Client::setTemperature(const double temp, const uint32_t time_stamp)
{
    temperature = temp;
    temperatureTimestamp = time_stamp;
}

/**
 * @brief Saves intervals for later exchange with the server.
 *
 * Depending on timestamp, @p intervals are either sent to the server or server's
 * intervals are downloaded.
 *
 * @param intervals ... intervals to be uploaded to the server but may be
 *                      overwritten by server's intervals.
 */
void Client::setIntervals(const IntervalList &intervals)
{
    intervalList = intervals;
}

void Client::initHost(const char *ip_addr, const uint16_t port)
{
    char port_str[7];
    std::sprintf(port_str, "%u", port);

    char *host_ptr = host;
    std::strcpy(host_ptr, ip_addr);
    host_ptr += std::strlen(ip_addr);
    *(host_ptr++) = ':';
    std::strcpy(host_ptr, port_str);
}

/**
 * Puts the respond into buffer and send it via TcpDriver.
 * @param request    ... response to send to the server.
 * @param await_body  ... whether next received message from server should contain body.
 * @return
 */
bool Client::send(const http::Request &request, bool await_body)
{
    char buffer[http::Request::TOTAL_SIZE];
    request.toBuffer(buffer);

    if (await_body) {
        http::ResponseBuffer::awaitBody();
    }

    return TcpDriver::send(reinterpret_cast<uint8_t *>(buffer), request.getSize());
}

/**
 * Reads connect response from the server. If the response is successfully read,
 * registered callback is called.
 * Note that response from server should contain server_real_time in body.
 *
 * @param response ... received response from the server.
 */
void Client::readConnectResponse(const http::Response &response)
{
    if (response.getStatusCode() != http::Response::OK) {
        // TODO: error handling
        sendConnectReq(deviceId);
        return;
    }

    // TODO: parse server_real_time from body.

    http::Request interval_timestamp_req = createIntervalTimestampReq();
    send(interval_timestamp_req, true);

    state = AWAIT_INTERVAL_TIMESTAMP_RESPONSE;

    callConnectedCb();
}

void Client::readIntervalTimestampResp(const http::Response &response)
{
    if (response.getStatusCode() != http::Response::OK) {
        // TODO: error handling
        send(createIntervalTimestampReq(), true);
        return;
    }

    uint32_t server_timestamp = static_cast<uint32_t>(std::atoi(
            reinterpret_cast<const char *>(response.getBody())
    ));
    // Temporarily store intervals timestamp from server.
    tempIntervalsTimestamp = server_timestamp;
    if (intervalList.getTimestamp() < server_timestamp) {
        // Download intervals from server.
        send(createGetIntervalsReq(), true);
        state = AWAIT_INTERVALS;
    }
    else if (intervalList.getTimestamp() > server_timestamp) {
        // Upload intervals to server.
        // TODO: what if intervals are empty?
        send(createPostIntervalsReq(intervalList), false);
        state = AWAIT_INTERVALS_ACK;
        callIntervalsSentCb();
    }
    else if (intervalList.getTimestamp() == server_timestamp) {
        send(createPostTemperature(temperature, temperatureTimestamp), false);
        state = AWAIT_TEMP_ACK;
        callTempSentCb();
    }
}

/**
 * Reads response from server that contains intervals. Note that it does not contain
 * timestamp.
 * @param response   ... response from the server.
 * @param time_stamp ... timestamp that was already sent from server.
 */
void Client::readIntervalsResp(const http::Response &response, const uint32_t time_stamp)
{
    if (response.getStatusCode() != http::Response::OK) {
        // TODO: error handling
        send(createGetIntervalsReq(), true);
        return;
    }

    IntervalList interval_list =
        IntervalList::deserialize(response.getBody(), response.getBodySize());

    intervalList = interval_list;
    intervalList.setTimestamp(time_stamp);
    callIntervalsRecvCb(intervalList);

    // Send temperature
    send(createPostTemperature(temperature, temperatureTimestamp), false);
    state = AWAIT_TEMP_ACK;
    callTempSentCb();
}

void Client::readIntervalsAckResp(const http::Response &response)
{
    if (response.getStatusCode() != http::Response::OK) {
        // TODO: error handling
        send(createPostIntervalsReq(intervalList), false);
        return;
    }

    // Send temperature
    send(createPostTemperature(temperature, temperatureTimestamp), false);
    state = AWAIT_TEMP_ACK;
    callTempSentCb();
}

void Client::readTempAckResp(const http::Response &response)
{
    if (response.getStatusCode() != http::Response::OK) {
        // TODO: error handling
        send(createPostTemperature(temperature, temperatureTimestamp), false);
        return;
    }

    //ClientTimer::start(2);
}

http::Request Client::createConnectReq(const char *device_id)
{
    return createPostReq(CONNECT_URL, device_id, std::strlen(device_id), "text/plain");
}

http::Request Client::createIntervalTimestampReq()
{
    return createGetReq(INTERVALS_TIMESTAMP_URL);
}

http::Request Client::createGetIntervalsReq()
{
    return createGetReq(INTERVALS_URL);
}

http::Request Client::createPostIntervalsReq(const IntervalList &interval_list)
{
    char timestamp_str[12];
    std::sprintf(timestamp_str, "%u", interval_list.getTimestamp());

    uint8_t buffer[IntervalList::MAX_SIZE];
    size_t buff_len = 0;
    interval_list.serialize(buffer, &buff_len);

    // Copy timestamp into body
    char body[IntervalList::MAX_SIZE + 13];
    char *body_it = body;
    std::strcpy(body_it, timestamp_str);
    body_it += std::strlen(timestamp_str);

    *body_it = '\n';
    body_it++;

    // Copy intervals into body
    std::memcpy(body_it, buffer, buff_len);
    body_it += buff_len;

    *body_it = '\0';

    return createPostReq(INTERVALS_URL, body, std::strlen(timestamp_str) + 1 + buff_len,
                         "application/octet-stream");
}

http::Request Client::createPostTemperature(const double temp, const uint32_t time_stamp)
{
    char timestamp_str[12];
    char temp_str[10];
    std::sprintf(temp_str, "%f", temp);
    std::sprintf(timestamp_str, "%u", time_stamp);
    size_t timestamp_len = std::strlen(timestamp_str);

    // concat timestamp and temp
    char body[25];
    std::strcpy(body, timestamp_str);
    *(body + timestamp_len) = '\n';
    std::strcpy(body + timestamp_len + 1, temp_str);

    return createPostReq(TEMP_URL, body, std::strlen(body), "text/plain");
}

http::Request Client::createGetReq(const char *url)
{
    using namespace http;

    Request request(Request::GET, url);
    HeaderOption hdr_option_host(HeaderOption::HOST, host);
    Header hdr;
    hdr.appendOption(hdr_option_host);
    request.appendHeader(hdr);

    return request;
}

http::Request Client::createPostReq(const char *url, const char *body, const size_t body_len,
                                    const char *content_type)
{
    using namespace http;

    char body_len_str[20];
    std::sprintf(body_len_str, "%lu", body_len);

    Request request(Request::POST, url);
    HeaderOption hdr_option_host(HeaderOption::HOST, host);
    HeaderOption hdr_option_content_type(HeaderOption::CONTENT_TYPE, content_type);
    HeaderOption hdr_option_content_length(HeaderOption::CONTENT_LENGTH, body_len_str);
    Header hdr;
    hdr.appendOption(hdr_option_host);
    hdr.appendOption(hdr_option_content_type);
    hdr.appendOption(hdr_option_content_length);
    request.appendHeader(hdr);

    request.appendBody(reinterpret_cast<const uint8_t *>(body), body_len);
    return request;
}

void Client::callConnectedCb()
{
    if (clientCbRecver != nullptr) {
        clientCbRecver->connectedCb();
    }
}

void Client::callTempSentCb()
{
    if (clientCbRecver != nullptr) {
        clientCbRecver->tempSentCb();
    }
}

void Client::callIntervalsSentCb()
{
    if (clientCbRecver != nullptr) {
        clientCbRecver->intervalsSentCb();
    }
}

void Client::callIntervalsRecvCb(const IntervalList &interval_list)
{
    if (clientCbRecver != nullptr) {
        clientCbRecver->intervalsRecvCb(interval_list);
    }
}

