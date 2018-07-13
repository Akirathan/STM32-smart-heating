/**
 * @file tcp_driver.cpp
 * @author mayfa
 * @date Jun 22, 2018
 */

#include "tcp_driver.hpp"
#include <cstring>   // For std::memcpy
#include "settings.h"
#include "rt_assert.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/lwip_timers.h"  // For sys_check_timeouts
#include "netif/etharp.h" // For ethernet_input
#include "ethernetif.h"
#include "http/response_buffer.hpp"
#include "client_timer.hpp" // For ClientTimer::checkTimeout
#include "application.hpp" // For Application::emitEvent
#include "communication_error_event.hpp"
#include "eth_link_up_event.hpp"

struct ip_addr  TcpDriver::destIpAddress;
uint16_t        TcpDriver::destPort = 0;
struct netif    TcpDriver::netInterface;
struct pbuf    *TcpDriver::writePacketBuffer = nullptr;
bool            TcpDriver::initialized = false;
bool            TcpDriver::linkUp = false;
struct tcp_pcb *TcpDriver::tmpTcpPcb = nullptr;

/**
 * Link status changed callback.
 * Redefined function from ethernetif.c
 * @param netif
 */
extern "C" void ethernetif_notify_conn_changed(struct netif *netif)
{
	if (netif_is_link_up(netif)) {
		TcpDriver::linkUpCallback();
	}
	else {
		TcpDriver::linkDownCallback();
	}
}

/**
 * Configures the network interface for LwIP.
 *
 * Given parameters represents destination IP address. TODO: Note that this address
 * will be later resolved by DNS.
 *
 * @param port ... destination port
 */
void TcpDriver::init(uint8_t ip_addr0, uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint16_t port)
{
	IP4_ADDR(&destIpAddress, ip_addr0, ip_addr1, ip_addr2, ip_addr3);
	destPort = port;

	// Init IP address.
	struct ip_addr ip;
	struct ip_addr netmask;
	struct ip_addr gw;
	// TODO: resolve these addresses with DHCP.
	IP4_ADDR(&ip, 192, 168, 0, 2);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
	IP4_ADDR(&gw, 192, 168, 0, 1);

	// Try to initialize the HW and add network interface.
	netif_add(&netInterface, &ip, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
	netif_set_link_callback(&netInterface, ethernetif_update_config);
	netif_set_default(&netInterface);

	if (netif_is_link_up(&netInterface)) {
		netif_set_up(&netInterface);
		linkUp = true;
	}
	else {
		netif_set_down(&netInterface);
	}

	initialized = true;
}

/**
 * Called when ETH link is connected.
 */
void TcpDriver::linkUpCallback()
{
	netif_set_up(&netInterface);
	linkUp = true;

	EthLinkUpEvent event;
	Application::emitEvent(event);
}

/**
 * Called when ETH link is disconnected.
 */
void TcpDriver::linkDownCallback()
{
	netif_set_down(&netInterface);
	linkUp = false;

	reset();

	CommunicationErrorEvent event(CommunicationErrorEvent::ETH_LINK_DOWN);
	Application::emitEvent(event);
}

bool TcpDriver::isLinkUp()
{
	return linkUp;
}

/**
 * Processes input from ethernet layer.
 * @note This function has to be called periodically from main loop.
 */
void TcpDriver::poll()
{
	// Update link status
	ethernetif_set_link(&netInterface);

	ethernetif_input(&netInterface);
	sys_check_timeouts();
	ClientTimer::checkTimeout();
}


bool TcpDriver::queueForSend(const uint8_t buff[], const size_t buff_size)
{
	rt_assert(initialized, "TcpDriver must be initialized before sending");
	rt_assert(linkUp, "ETH link must be up before sending");

	// Copy data into packet buffer (pbuf).
	writePacketBuffer = pbuf_alloc(PBUF_TRANSPORT, static_cast<uint16_t>(buff_size), PBUF_POOL);
	if (writePacketBuffer == nullptr) {
		return false;
	}
	err_t err = pbuf_take(writePacketBuffer, buff, buff_size);
	if (err != ERR_OK) {
		return false;
	}

	struct tcp_pcb * pcb;
	pcb = tcp_new();
	err = tcp_connect(pcb, &destIpAddress, destPort, connectedCb);
	if (err != ERR_OK) {
		return false;
	}

	// Set error callback
	tcp_err(pcb, errorCb);

	return true;
}

/**
 * Called from @ref Client when whole HTTP message was received.
 *
 * This is good opportunity to shut down the connection for example.
 */
void TcpDriver::wholeMessageReceivedCb()
{

}

/**
 * This function is called when TCP connection is established.
 */
err_t TcpDriver::connectedCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	tcp_recv(tpcb, receivedCb);
	tcp_sent(tpcb, sentCb);
	tmpTcpPcb = tpcb;

	while (writePacketBuffer != nullptr && writePacketBuffer->len <= tcp_sndbuf(tpcb)) {
		err_t err = tcp_write(tpcb, writePacketBuffer->payload, writePacketBuffer->len, TCP_WRITE_FLAG_COPY);
		if (err != ERR_OK) {
			return err;
		}

		struct pbuf *old_pbuff = writePacketBuffer;
		writePacketBuffer = writePacketBuffer->next;
		if (writePacketBuffer != nullptr) {
			pbuf_ref(writePacketBuffer);
		}
		pbuf_free(old_pbuff);
	}
	return ERR_OK;
}

/**
 * This function is called when some data were sent over TCP.
 * @param arg
 * @param tpcb
 * @param len
 * @return
 */
err_t TcpDriver::sentCb(void *arg, struct tcp_pcb *tpcb, uint16_t len)
{
	// Close previous connection
	if (tmpTcpPcb != nullptr) {
		tcp_shutdown(tmpTcpPcb, 0, 1);
		tmpTcpPcb = nullptr;
	}

	return ERR_OK;
}

/**
 * This function is called when some data were received from TCP.
 * @param packet_buff ... received packet buffer
 */
err_t TcpDriver::receivedCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *packet_buff, err_t err)
{
	// Check if connection was closed by the other end.
	if (packet_buff == nullptr) {
		disconnect(tpcb);
		return ERR_OK;
	}

	// Received non-empty frame but there is some error.
	if (err != ERR_OK) {
		pbuf_free(packet_buff);
		return err;
	}

	processReceivedData(packet_buff);

	tcp_recved(tpcb, packet_buff->tot_len);

	pbuf_free(packet_buff);
	return ERR_OK;
}

void TcpDriver::errorCb(void *arg, err_t err)
{
	reset();

	CommunicationErrorEvent event(CommunicationErrorEvent::TCP_ERROR);
	Application::emitEvent(event);
}

// Copies received data into dummyReceiveBuffer for now.
void TcpDriver::processReceivedData(struct pbuf *packet_buff)
{
	uint8_t receive_buff[1500];
	size_t receive_buff_idx = 0;

	// Copy from packet_buff to receive_buff.
	while (packet_buff != nullptr && packet_buff->len + receive_buff_idx < 1500) {
		std::memcpy(receive_buff + receive_buff_idx, packet_buff->payload, packet_buff->len);
		receive_buff_idx += packet_buff->len;
		packet_buff = packet_buff->next;
	}

	http::ResponseBuffer::buff(receive_buff, receive_buff_idx);

	// All data from packet_buff should be copied because LwIP does not support
	// IP fragmentation and therefore cannot exceed 1500 bytes.
	rt_assert(packet_buff == nullptr, "All data from packet_buff should be copied");
}

void TcpDriver::disconnect(struct tcp_pcb *tpcb)
{
	tcp_recv(tpcb, nullptr);
	tcp_sent(tpcb, nullptr);
	tcp_close(tpcb);
}

void TcpDriver::reset()
{
	if (writePacketBuffer != nullptr) {
		pbuf_free(writePacketBuffer);
	}
}
