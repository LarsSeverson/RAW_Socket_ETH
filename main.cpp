#include "frameio.h"
#include "message.h"
#include <cstdio>
#include <pthread.h>
#include <iostream>

frameio net;
message arp;

pthread_t loop_thread, arp_thread;


void* protocol_loop(void* arg){
	frame buf;
	while(1){
		int n = net.rec_frame(&buf, sizeof(buf));
		if(n >= 42 && (buf.protocol[0] << 8 | buf.protocol[1]) == 0x806){
			arp.send(PACKET, buf.payload, n);
		}
	}
}
void* arp_loop(void* arg){
	_uc buf[1500];
	event_type event;
	while(1){
		arp.receive(&event, buf, sizeof(buf));
		printf("got an ARP %s\n", buf[7]==1? "request":"reply");
		for(int i = 0; i < 42; ++i){
			printf("%02x ", buf[i]);
			if((i + 1) == 22){
				printf("\n");
			}
		}
		printf("\n");
	}
}

int main(){
	int success = net.open_net("ens33");
	if(success == -1){
		printf("Could not open device... (ifconfig).");
		return 0;
	}
	pthread_create(&loop_thread, NULL, protocol_loop, NULL);
	pthread_create(&arp_thread, NULL, arp_loop, NULL);
	while(1){}
	return 0;
}
