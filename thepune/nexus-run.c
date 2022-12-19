#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<pthread.h>
#include<mosquitto.h>

extern int publish();
extern void listento();

void *runListener(struct mosquitto *mosq){
    listento(mosq);
}

int main(){
    pthread_t server_subber;
    struct mosquitto *mosq;
    int ss = pthread_create(&server_subber, NULL, runListener, mosq);
    sleep(5);
    float v = 12.35;
    printf("%f\n", v);
    publish(v, mosq);
    
    pthread_join(server_subber, NULL);
    /*mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();*/
    return 0;
}