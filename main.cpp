#include "frameio.h"
#include "message.h"
#include <cstdio>
#include <pthread.h>
#include <iostream>

frameio net;
message arp;

int count = 1;
frame frame_buf;
_uc buf[1500];

void get_arp(){
	arp.receive(buf, sizeof(buf));
	printf("%d. ", count++);
	printf("got an ARP %s\n", buf[7]==1? "request":"reply");
	for(int i = 0; i < 42; ++i){
		printf("%02x ", buf[i]);
		if((i + 1) == 22){
			printf("\n");
		}
	}
	printf("\n");
}

void protocol_loop(){
	int n = net.rec_frame(&frame_buf, sizeof(frame_buf));
		if(n >= 42 && (frame_buf.protocol[0] << 8 | frame_buf.protocol[1]) == 0x806){
			arp.send(frame_buf.payload, n);
			get_arp();
		}
}

int main(){
	int success = net.open_net("ens33");
	if(success == -1){
		printf("Could not open device... (ifconfig).");
		return 0;
	}
	while(1){
		protocol_loop();
	}
	return 0;
}
