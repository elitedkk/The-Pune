#include<mosquitto.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern changeAction(int act);

int on_connect(struct mosquitto *mosq, void *obj, int rc){
    if(rc != 0){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
        return -1;
    }
    mosquitto_subscribe(mosq, NULL, "/action", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
	if(strcmp(msg->topic, "/action") == 0){
		int action = 0;
		action = atoi((char *) msg->payload);
		//printf("Topic: %s; Received message: %s\n", msg->topic, (char *) msg->payload);
		printf("Received: %d\n", action);
		changeAction(action);
	}
}


void listento(struct mosquitto *mosq){
    int rc;
    int deviceID = 0;
    mosquitto_lib_init();
    //struct mosquitto *mosq;
    
    mosq = mosquitto_new("raspi", true, &deviceID);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "10.0.0.75", 1883, 60);
    if(rc){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
    }
    
    printf("Connected to the broker\n");
    //mosquitto_publish(mosq, NULL, "/dist", 1, "5", 0, false);
    
    mosquitto_loop_start(mosq);
    getchar();
    mosquitto_loop_stop(mosq, true);
    

/*
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    */
}
