#include<stdio.h>
#include <pthread.h>
#include <mosquitto.h>
#include <unistd.h>
#include <stdlib.h>

extern publish(int dist, struct mosquitto *mosq);
extern listento();
extern publishBMP();

extern dolphin_setup();
extern sonar();

extern motor_setup();

extern void getBMPValues(float *avg_temp, long *avg_p, float *avg_alt);




void *runListener(struct mosquitto *mosq){
	listento(mosq);
}

struct mosquitto *mosq;


void *runBMP(void *args){
	float avg_temp = 0;
	float avg_alt = 0;
	long avg_p = 0;
	while(1){
		getBMPValues(&avg_temp, &avg_p, &avg_alt);
		usleep(3 * 1000 * 1000);
		publishBMP(avg_temp, avg_p, avg_alt, mosq);
	}
}

void *runSonar(void * args){
	long dist;
	while(1){
		dist = sonar();
		publish(dist, mosq);
	}
}

int main(){
	pthread_t raspi_subber, bmpthread, sonarthread;
    	//struct mosquitto *mosq;
    	int rs = pthread_create(&raspi_subber, NULL, runListener, mosq);
    	sleep(5);
    	float v = 12.35;

	dolphin_setup();
	motor_setup();

	int bt = pthread_create(&bmpthread, NULL, runBMP, (void *) NULL);
	int so = pthread_create(&sonarthread, NULL, runSonar, (void *) NULL);
	
	pthread_join(raspi_subber, NULL);
	pthread_join(bmpthread, NULL);
	pthread_join(sonarthread, NULL);
	//printf("The Pune: temp = %f\n", avg_temp);
}

