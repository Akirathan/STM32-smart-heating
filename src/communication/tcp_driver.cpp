/**
 * @file tcp_driver.cpp
 * @author mayfa
 * @date Jun 22, 2018
 */

#include "tcp_driver.hpp"
#include "settings.h"
#include "rt_assert.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "ethernetif.h"

struct ip_addr TcpDriver::destIpAddress;
uint16_t       TcpDriver::destPort = 0;
struct netif   TcpDriver::netInterface;
struct pbuf   *TcpDriver::writePacketBuffer = nullptr;
bool           TcpDriver::initialized = false;

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
	netif_add(&netInterface, &ip, &netmask, &gw, NULL, &ethernetif_init,
			(void (*) (netif *))&ethernetif_input);
	netif_set_default(&netInterface);
	netif_set_up(&netInterface);
	netif_set_link_callback(&netInterface, ethernetif_update_config);

	initialized = true;
}


bool TcpDriver::queueForSend(uint8_t buff[], const size_t buff_size)
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
	err = tcp_connect(pcb, &destIpAddress, destPort, TcpDriver::connectedCb);
	if (err != ERR_OK) {
		return false;
	}
	return true;
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


