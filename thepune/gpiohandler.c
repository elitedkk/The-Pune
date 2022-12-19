#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <time.h>
#include <sys/time.h>

#define OUTPUT "out"
#define INPUT "in"
#define LOW "0"
#define HIGH "1"

#define TRIG "18"
#define ECHO "24"

#define ON 49
#define OFF 48

#define P1 "16"
#define P2 "20"
#define N1 "21"
#define N2 "26"

#define SLOW 20
#define FAST 60

static int action = 0;
int lock = 0;

void pinMode(char pin[], char mode[]) {
	FILE *sysfs_export;
	FILE *sysfs_direction;
	char path[40] = "";
	sysfs_export = fopen("/sys/class/gpio/export", "w");
	fwrite(pin, 1, sizeof(pin), sysfs_export);
	fclose(sysfs_export);

	strcpy(path, "/sys/class/gpio/gpio");
	strcat(path, pin);
	strcat(path, "/direction");

	sysfs_direction = fopen(path, "w");
	fwrite(mode, 1, sizeof(mode), sysfs_direction);
	fclose(sysfs_direction);
}

void digitalWrite(char pin[], char value[]) {
	char path[40];
	FILE *sysfs_value;
	strcpy(path, "/sys/class/gpio/gpio");
	strcat(path, pin);
	strcat(path, "/value");

	sysfs_value = fopen(path, "w");
	fwrite(value, 1, sizeof(value), sysfs_value);
	fclose(sysfs_value);
}

char digitalRead(char pin[]){
	char path[40];
	FILE *sysfs_value;
	strcpy(path, "/sys/class/gpio/gpio");
	strcat(path, pin);
	strcat(path, "/value");
	sysfs_value = fopen(path, "r");
	char value;
	fread(&value, 1, 1, sysfs_value);
	fclose(sysfs_value);
	return value;
}

void cleanUp(char pin[]) {
	FILE *sysfs_unexport;
	sysfs_unexport = fopen("/sys/class/gpio/unexport", "w");
	fwrite(pin, 1, sizeof(pin), sysfs_unexport);
	fclose(sysfs_unexport);
}

void dolphin_setup(){
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
}

void motor_setup(){
	pinMode(P1, OUTPUT);
	pinMode(P2, OUTPUT);
	pinMode(N1, OUTPUT);
	pinMode(N2, OUTPUT);
	allLow();
}

long sonar(){
	struct timeval begin, end;
//	while(1){
		digitalWrite(TRIG, HIGH);
		usleep(10);
		digitalWrite(TRIG, LOW);
			while(digitalRead(ECHO) == OFF){
				gettimeofday(&begin, NULL);
			}
			printf("RECEIVED ON\n");
			while(digitalRead(ECHO) == ON){
				gettimeofday(&end, NULL);
			}
			printf("PROCESS ON\n");
			long timetaken = (1000000 * (end.tv_sec - begin.tv_sec)) + ((end.tv_usec - begin.tv_usec));
			long dist = (timetaken * 343 / 10000)/2;
			printf("Distance for the sensor = %ld cm\n", dist);
			usleep(2000 * 1000);
//	}
	return dist;
}

void echo(){
	dolphin_setup();
	sonar();
}

void allLow(){
	digitalWrite(P1, LOW);
	digitalWrite(P2, LOW);
	digitalWrite(N1, LOW);
	digitalWrite(N2, LOW);
}

void move(char pin[], int dutyCycle){
	const int gcf = 10;
	digitalWrite(pin, HIGH);
	usleep(1000*dutyCycle*gcf);
	digitalWrite(pin, LOW);
	usleep(1000*(100-dutyCycle)*gcf);
}

void run_motor(char pin[]){
	while(1){
		if(lock == 0){ continue;}
		if(action == 0){
			allLow();
			printf("Don't move\n");
		}
		else if(action > 0){
			if(action == 1){
				move(P1, SLOW);
				move(N2, SLOW);
			}
			else if(action == 2){
				move(P1, FAST);
				move(N2, FAST);
			}
		}
		else if(action < 0){
			if(action == -1){
				move(P2, SLOW);
				move(N1, SLOW);
			}
			else if(action == -2){
				move(P2, FAST);
				move(N1, FAST);
			}
		}
		//if (lock == 1) ;
	}
}

void changeAction(int act){
	lock = 1;
	printf("Action = %d\n", act);
	allLow();
	usleep(100);
	action = act;
	lock = 0;
}

/*
int main(int argc, char **argv) {
	int act = argv[1];
	int dutyCycle = 0;
	char action = "0";
	return 0;
}
*/
