#pragma once
#include <sys/socket.h>
#include <unistd.h>

typedef unsigned char _uc;

class frameio
{
public:
	frameio(){
		sock = -1;
	}
	~frameio(){
		sock >= 0 ? close(sock) : sock = -1;
	}
	int send_frame(void* frame, int len){
		return write(sock, frame, len);
	}
	int rec_frame(void* frame, int max_len){
		return recvfrom(sock, frame, max_len, 0, NULL, NULL);
	}
	int open_net(const char* device);

	const _uc* get_mac(){
		return my_mac;
	}

private:
	_uc my_mac[6];
	int sock;

};


struct frame{
	_uc dst_mac[6];
	_uc src_mac[6];
	_uc protocol[2];
	_uc payload[1500];
};
