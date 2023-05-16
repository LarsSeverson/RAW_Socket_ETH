#pragma once
#include <unistd.h>
#include <pthread.h>

typedef enum{PACKET, TIMER} event_type;

class message
{
public:
    message(){
        pipe(fd);
        pthread_mutex_init(&mutex, NULL);
    }
    ~message(){
        pthread_mutex_destroy(&mutex);
        close(fd[0]);
        close(fd[1]);
    }

    void send(event_type event, void* buf, int len);
    int receive(event_type* event, void* buf, int max_len);

private:
    pthread_mutex_t mutex;
    int fd[2];
};