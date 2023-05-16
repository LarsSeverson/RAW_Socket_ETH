#pragma once
#include <unistd.h>

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

    void send(void* buf, int len);
    int receive(void* buf, int max_len);

private:
    int fd[2];
};