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
#include "lwip/dhcp.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

class TcpDriver {
public:
	static void init(uint16_t port);
	static void linkUpCallback();
	static void linkDownCallback();
	static void statusChangedCallback(struct netif *netif);
	static bool isLinkUp();
	static void poll();
	static bool queueForSend(const uint8_t buff[], const size_t buff_size);
	static void wholeMessageReceivedCb();
private:
	/// Fallback IP address when DHCP module fails.
	const static uint32_t ip[4];
	const static uint32_t destinationIp[4];
	const static uint32_t netMask[4];
	const static uint32_t gw[4];
	const static char *destinationHost;

	static struct ip_addr destIpAddress;
	static uint16_t destPort;
	static struct netif netInterface;
	static struct dhcp dhcp;
	static struct pbuf *writePacketBuffer;
	static bool initialized;
	static bool linkUp;
	/// True when any address was set to netInterface. This flag is needed because
	/// DHCP module may not set the address.
	static bool addressesSet;
	static bool destinationResolved;
	/// Temporary storage for TCP PCB, needed when @ref Client calls
	/// @ref wholeMessageReceivedCb and we want to disconnect.
	static struct tcp_pcb *tmpTcpPcb;


	static void dnsFoundCallback(const char *name, ip_addr_t *ip_addr, void *arg);
	static err_t connectedCb(void *arg, struct tcp_pcb *tpcb, err_t err);
	static err_t sentCb(void *arg, struct tcp_pcb *tpcb, uint16_t len);
	static err_t receivedCb(void *arg, struct tcp_pcb *tpcb, struct pbuf *packet_buff, err_t err);
	static void errorCb(void *arg, err_t err);
	static void processReceivedData(struct pbuf *packet_buff);
	static void disconnect(struct tcp_pcb *tpcb);
	static void reset();
	static void manualSetAddress();
};


#endif /* TCP_DRIVER_HPP_ */
