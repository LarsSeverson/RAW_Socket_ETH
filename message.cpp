#include "message.h"
#include <cstdio>

void message::send(event_type event, void* buf, int len){
    if ((unsigned) len > 0xffffff){
        len = 0;
    }
    
    int header = event << 24 | len;
    
    write(fd[1], &header, sizeof(int));
    write(fd[1], buf, len);
}

int message::receive(event_type* event, void* buf, int max_len){
    int len, header;
    read(fd[0], &header, sizeof(int));
    *event = event_type(header>>24);
    len = header & 0xffffff;

    if(max_len > len){
        max_len = len;
    }
    read(fd[0], buf, max_len);

    while(len-- > max_len){
        read(fd[0], &header, 1);
    }
    return max_len;
}