#include<mosquitto.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int publish(long dist, struct mosquitto *mosq){
    char val[10 + sizeof(char)];
    //printf("%f\n", dist);
    //gcvt(dist, 9, val);
    //printf("%s\n", val);
    int rc;
    int deviceId = 0;
    //mosquitto_lib_init();
    //struct mosquitto *mosq;
    mosq = mosquitto_new("server-publisher", true, &deviceId);
    rc = mosquitto_connect(mosq, "10.0.0.75", 1883, 60);
    if(rc != 0){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("Publishing distance to the broker\n");
    

    sprintf(val, "%ld", dist);
    int pub_result = mosquitto_publish(mosq, NULL, "/dist", 2, val, 1, false);
    if(pub_result == MOSQ_ERR_SUCCESS){
        printf("Published to the broker\n");
    }
    else{
        printf("Publish not successful\n");
    }
    

    //mosquitto_disconnect(mosq);
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
    return 0;
}

int publishBMP(float t, long p, float alt, struct mosquitto *mosq){
    char val_temp[20];
    char val_p[20];
    char val_alt[20];
    //printf("%f\n", dist);
    //gcvt(dist, 9, val);
    
    //printf("%s\n", val);
    
    int rc;
    int deviceId = 0;
    //mosquitto_lib_init();
    //struct mosquitto *mosq;
    mosq = mosquitto_new("server-publisher", true, &deviceId);
    rc = mosquitto_connect(mosq, "10.0.0.75", 1883, 60);
    if(rc != 0){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    } 
    printf("Publishing bmp to the broker\n");
    

    sprintf(val_temp, "%f", t);
    sprintf(val_p, "ld", p);
    sprintf(val_alt, "%f", alt);
    printf("%f %ld %f", val_temp, val_p, val_alt);
    int pub_t = mosquitto_publish(mosq, NULL, "/temp", 10, val_temp, 1, false);
    if(pub_t == MOSQ_ERR_SUCCESS){
        printf("Published to the broker\n");
    }
    else{
        printf("Publish not successful\n");
    }

    int pub_p = mosquitto_publish(mosq, NULL, "/press", 10, val_p, 1, false);
    if(pub_p == MOSQ_ERR_SUCCESS){
        printf("Published to the broker\n");
    }
    else{
        printf("Publish not successful\n");
    }

    int pub_alt = mosquitto_publish(mosq, NULL, "/alt", 10, val_alt, 1, false);
    if(pub_alt == MOSQ_ERR_SUCCESS){
        printf("Published to the broker\n");
    }
    else{
        printf("Publish not successful\n");
    }

    //mosquitto_disconnect(mosq);
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
    return 0;
}
