/**
 * @file tcp_driver.hpp
 * @author mayfa
 * @date Jun 22, 2018
 */

#ifndef TCP_DRIVER_HPP_
#define TCP_DRIVER_HPP_

#include <cstdint>
#include <cstddef>
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

class TcpDriver {
public:
	static void init(uint8_t ip_addr0, uint8_t ip_addr1, uint8_t ip_addr2, uint8_t ip_addr3, uint16_t port);
	static void linkUpCallback();
	static void linkDownCallback();
	static void poll();
	static bool queueForSend(const uint8_t buff[], const size_t buff_size);
	static void wholeMessageReceivedCb();
private:
	static struct ip_addr destIpAddress;
	static uint16_t destPort;
	static struct netif netInterface;
	static struct pbuf *writePacketBuffer;
	static bool initialized;
	static bool linkUp;
	/// Temporary storage for TCP PCB, needed when @ref Client calls
	/// @ref wholeMessageReceivedCb and we want to disconnect.
	static struct tcp_pcb *tmpTcpPcb;

	static err_t connectedCb(void *arg, struct tcp_pcb *tpcb, err_t err);
	static err_t sentCb(void *arg, struct tcp_pcb *tpcb, uint16_t len);
	static err_t receivedCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *packet_buff, err_t err);
	static void errorCb(void *arg, err_t err);
	static void processReceivedData(struct pbuf *packet_buff);
	static void disconnect(struct tcp_pcb *tpcb);
	static void reset();
};


#endif /* TCP_DRIVER_HPP_ */
