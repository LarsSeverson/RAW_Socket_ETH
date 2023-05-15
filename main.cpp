#include "frameio.h"
#include <cstdio>

frameio net;

struct frame{
	_uc dst_mac[6];
	_uc src_mac[6];
	_uc protocol[2];
	_uc payload[1500];
};

void protocol_loop(){
	frame buf;
	int n = net.rec_frame(&buf, sizeof(buf));
	if(n >= 42){
		if(buf.protocol[0] << 8 | buf.protocol[1] == 0x806){
			printf("ARP packet: \n");	
			for (int i = 0; i < 42; ++i){
				printf("%02X ", buf.payload[i]);

				if((i+1) % 22 == 0){
					printf("\n");
				}
			}
		}
	}
}

int main(){
	int success = net.open_net("ens33");
	if(success == -1){
		printf("Could not open device... (ifconfig).");
		return 0;
	}

	while(true){
		protocol_loop();
		printf("\n");
	}
	return 0;
}
