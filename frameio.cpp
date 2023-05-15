#include "frameio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <string.h>
#include <netinet/in.h>

int frameio::open_net(const char* device){
	const int len = 80;
	struct sockaddr_ll sll;
	struct ifreq if_req;

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = PF_PACKET;
	sll.sll_protocol = htons(ETH_P_ALL);

	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock < 0) return sock;

	// assign the device name to if_req
	strcpy(if_req.ifr_name, device);
	ioctl(sock, SIOCGIFINDEX, &if_req);
	sll.sll_ifindex = if_req.ifr_ifindex;

	bind(sock, (struct sockaddr*)&sll, sizeof(sll));

	struct ifreq ifr;
	strncpy(&ifr.ifr_name[0], device, IFNAMSIZ);
	if(ioctl(sock, SIOCGIFHWADDR, &ifr) >= 0){
		memcpy(my_mac, &ifr.ifr_hwaddr.sa_data, 6);
		return sock;
	}

	close(sock);
	return sock = -1;
}
