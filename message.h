#pragma once
#include <unistd.h>

typedef enum{PACKET, TIMER} event_type;

class message
{
public:
    message(){
        pipe(fd);
    }
    ~message(){

        close(fd[0]);
        close(fd[1]);
    }

    void send(event_type event, void* buf, int len);
    int receive(event_type* event, void* buf, int max_len);

private:
    int fd[2];
};