/**
 * @file client.cpp
 * @author mayfa
 * @date Jun 25, 2018
 */

#include "rt_assert.h"
#include "client.hpp"
#include "client_timer.hpp"
#include "client_error_timer.hpp"
#include "client_receive_timeout_timer.hpp"
#include <cstdio> // For std::sprintf
#include <cstdlib> // For std::atoi
#include "http/response_buffer.hpp"
#include "tcp_driver.hpp"
#include "sprintf_double.hpp"
#include "des.hpp" // For DES encryption

bool                  Client::initialized = false;
bool                  Client::connected = false;
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
 * @param host
 * @param port
 * @param client_cb_recver ... receiver of all client callbacks, may be NULL.
 */
void Client::init(const char *host, uint16_t port, IClientCbRecver *client_cb_recver)
{
    if (!initialized) {
        Client::port = port;
        initHost(host, port);
        clientCbRecver = client_cb_recver;
        initialized = true;
    }
}

void Client::receiveCb(http::Response &response)
{
	rt_assert(initialized, "Client must be initialized before receiving");
    rt_assert(connected, "Client must be connected before receiving anything");

    if (response.getStatusCode() != http::Response::OK) {
    	handleError();
    }

    decryptResponseBody(response);

    switch (state) {
        case AWAIT_CONNECT_RESPONSE:
            readConnectResponse(response);
            break;
        case AWAIT_INTERVAL_TIMESTAMP_RESPONSE:
            readIntervalTimestampResp(response);
            break;
        case AWAIT_INTERVALS:
        	removePaddingFromIntervalsDecryption(response);
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
 * @param device_id ... ID of this device
 * @return false when TCPDriver fails to send data.
 */
bool Client::sendConnectReq(const char *device_id)
{
	rt_assert(initialized, "Client must be initialized first");

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
	rt_assert(initialized, "Client must be initialized first");

    send(createIntervalTimestampReq(), true);
    state = AWAIT_INTERVAL_TIMESTAMP_RESPONSE;
    callTempSentCb();
}

/**
 * @brief Disconnects from the server.
 *
 * After Client is disconnected, @ref sendConnectReq must be called to connect this
 * Client again.
 */
void Client::disconnect()
{
    connected = false;
    state = AWAIT_NONE;
}

/**
 * Start @ref ClientErrorTimer countdown so the connection is tried again
 * in some time.
 *
 * @note This method is called when a connection error occurs.
 */
void Client::handleError()
{
	connected = false;
	state = AWAIT_NONE;

	ClientErrorTimer::start();
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

void Client::initHost(const char *host, const uint16_t port)
{
    char port_str[7];
    std::sprintf(port_str, "%u", port);

    char *host_ptr = Client::host;
    std::strcpy(host_ptr, host);
    host_ptr += std::strlen(host);
    *(host_ptr++) = ':';
    std::strcpy(host_ptr, port_str);
}

/**
 * Puts the respond into buffer and send it via TcpDriver. Also encrypts body
 * of the request before sending it.
 * @param request    ... response to send to the server.
 * @param await_body  ... whether next received message from server should contain body.
 * @return
 */
bool Client::send(http::Request request, bool await_body)
{
    if (await_body) {
        http::ResponseBuffer::awaitBody();
    }

    encryptRequestBody(request);

    ClientReceiveTimeoutTimer::start();

    char buffer[http::Request::TOTAL_SIZE] = {0};
    request.toBuffer(buffer);
    return TcpDriver::queueForSend(reinterpret_cast<uint8_t *>(buffer), request.getSize());
}

void Client::encryptRequestBody(http::Request &request)
{
	if (request.getBodyLen() == 0) {
		return;
	}

    // Encrypt and reset request's body.
    int32_t enc_body_len = 0;
    uint8_t enc_body[DES::MAX_BUFFER_SIZE];
    DES::encrypt(request.getBody(), request.getBodyLen(), enc_body, &enc_body_len);
    request.appendBody(enc_body, enc_body_len);

    // Reset Content-Length.
    char enc_body_len_str[10];
    std::sprintf(enc_body_len_str, "%ld", enc_body_len);
    request.getHeader().setOptionValue(http::HeaderOption::CONTENT_LENGTH, enc_body_len_str);
}

void Client::decryptResponseBody(http::Response &response)
{
	if (response.getBodySize() == 0) {
		return;
	}

    int32_t decrypted_body_size = 0;
    uint8_t decrypted_body[DES::MAX_BUFFER_SIZE];
    DES::decrypt(response.getBody(), response.getBodySize(), decrypted_body, &decrypted_body_size);

    response.copyIntoBody(decrypted_body, decrypted_body_size);
}

/**
 * Used only for response that contains intervals configuration.
 * Therefore it is mandatory that the trailing zero-bytes padding is removed
 * until response's body length is a multiply of 12 (serialized interval length).
 *
 * @param response ... body of this response will be modified and length of
 *                     this body will be a multiple of 12.
 */
void Client::removePaddingFromIntervalsDecryption(http::Response &response)
{
	if ((response.getBodySize() % Interval::SIZE) == 0) {
		return;
	}

    size_t i = response.getBodySize() - 1;
    while (i > 0 && response.getBody()[i] == 0 && (i % Interval::SIZE) != 0) {
    	i--;
    }
    size_t size_without_padding = i;

    response.copyIntoBody(response.getBody(), size_without_padding);
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
    	handleError();
        return;
    }

    uint32_t server_real_time = static_cast<uint32_t>(std::atoi(
            reinterpret_cast<const char *>(response.getBody())
    ));

    http::Request interval_timestamp_req = createIntervalTimestampReq();
    send(interval_timestamp_req, true);

    state = AWAIT_INTERVAL_TIMESTAMP_RESPONSE;

    callConnectedCb(server_real_time);
}

void Client::readIntervalTimestampResp(const http::Response &response)
{
    if (response.getStatusCode() != http::Response::OK) {
    	handleError();
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
    	handleError();
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
    	handleError();
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
    	handleError();
        return;
    }

    ClientTimer::start();
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
	uint8_t body[4 + IntervalList::MAX_SIZE];
	*(reinterpret_cast<uint32_t *>(body)) = interval_list.getTimestamp();

	size_t intervals_len = 0;
	interval_list.serialize(body + 4, &intervals_len);

    return createPostReq(INTERVALS_URL, reinterpret_cast<char *>(body), 4 + intervals_len,
                         "application/octet-stream");
}

http::Request Client::createPostTemperature(const double temp, const uint32_t time_stamp)
{
    char timestamp_str[12];
    char temp_str[10];
    sprintf_double(temp_str, temp, 4);
    std::sprintf(timestamp_str, "%lu", time_stamp);
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
    std::sprintf(body_len_str, "%u", body_len);

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

void Client::callConnectedCb(uint32_t server_real_time)
{
    if (clientCbRecver != nullptr) {
        clientCbRecver->connectedCb(server_real_time);
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
