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
	static void poll();
	static bool queueForSend(const uint8_t buff[], const size_t buff_size);
private:
	static struct ip_addr destIpAddress;
	static uint16_t destPort;
	static struct netif netInterface;
	static struct pbuf *writePacketBuffer;
	static bool initialized;
	static uint8_t dummyReceiveBuffer[512];

	static err_t connectedCb(void *arg, struct tcp_pcb *tpcb, err_t err);
	static err_t sentCb(void *arg, struct tcp_pcb *tpcb, uint16_t len);
	static err_t receivedCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *packet_buff, err_t err);
	static void processReceivedData(struct pbuf *packet_buff);
	static void disconnect(struct tcp_pcb *tpcb);
};


#endif /* TCP_DRIVER_HPP_ */
