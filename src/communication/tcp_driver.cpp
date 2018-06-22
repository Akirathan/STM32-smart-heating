/**
 * @file tcp_driver.cpp
 * @author mayfa
 * @date Jun 22, 2018
 */

#include "tcp_driver.hpp"
#include "settings.h"
#include "lwip/init.h"
#include "ethernetif.h"

void TcpDriver::init()
{
	struct netif net;

	// Init IP address.
	struct ip_addr ip;
	struct ip_addr netmask;
	struct ip_addr gw;
	IP4_ADDR(&ip, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
	IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

	// Config.
	netif_add(&net, &ip, &netmask, &gw, NULL, &ethernetif_init,
			(void (*) (netif *))&ethernetif_input);
	netif_set_default(&net);
	netif_set_up(&net);
	netif_set_link_callback(&net, ethernetif_update_config);
}
