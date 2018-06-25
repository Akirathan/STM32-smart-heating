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

struct ip_addr  TcpDriver::destIpAddress;
uint16_t        TcpDriver::destPort = 0;
struct netif    TcpDriver::netInterface;
struct pbuf    *TcpDriver::writePacketBuffer = nullptr;
bool            TcpDriver::initialized = false;
struct tcp_pcb *TcpDriver::tmpTcpPcb = nullptr;

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

	// Config.
	netif_add(&netInterface, &ip, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
	netif_set_default(&netInterface);
	netif_set_up(&netInterface);
	netif_set_link_callback(&netInterface, ethernetif_update_config);

	initialized = true;
}

/**
 * Processes input from ethernet layer.
 * @note This function has to be called periodically from main loop.
 */
void TcpDriver::poll()
{
	ethernetif_input(&netInterface);
	sys_check_timeouts();
}


bool TcpDriver::queueForSend(const uint8_t buff[], const size_t buff_size)
{
	rt_assert(initialized, "TcpDriver must be initialized before sending");

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
	return true;
}

/**
 * Called from @ref Client when whole HTTP message was received.
 *
 * This is good opportunity to shut down the connection for example.
 */
void TcpDriver::wholeMessageReceivedCb()
{
	rt_assert(tmpTcpPcb != nullptr, "tmpTcpPcb should be sent somewhen after receivedCb");

	disconnect(tmpTcpPcb);
}

/**
 * This function is called when TCP connection is established.
 */
err_t TcpDriver::connectedCb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	tcp_recv(tpcb, receivedCb);
	tcp_sent(tpcb, sentCb);

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
	// TODO: remove this function?
	return ERR_OK;
}

/**
 * This function is called when some data were received from TCP.
 * @param packet_buff ... received packet buffer
 */
err_t TcpDriver::receivedCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *packet_buff, err_t err)
{
	if (packet_buff == nullptr) {
		disconnect(tpcb);
		return ERR_OK;
	}

	// Received non-empty frame but there is some error.
	if (err != ERR_OK) {
		pbuf_free(packet_buff);
		return err;
	}

	tcp_recved(tpcb, packet_buff->tot_len);

	tmpTcpPcb = tpcb;
	processReceivedData(packet_buff);

	pbuf_free(packet_buff);
	return ERR_OK;
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
