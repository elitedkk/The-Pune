#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define OUTPUT "out"
#define LOW "0"
#define HIGH "1"

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

void cleanUp(char pin[]) {
	FILE *sysfs_unexport;
	sysfs_unexport = fopen("/sys/class/gpio/unexport", "w");
	fwrite(pin, 1, sizeof(pin), sysfs_unexport);
	fclose(sysfs_unexport);
}

void setLed(int dutyCycle){
	const int gcf = 10;
	pinMode("5", OUTPUT);
	digitalWrite("5", HIGH);
	//sleep(5);
	usleep(1000*dutyCycle*gcf);
	digitalWrite("5", LOW);
	usleep((100-dutyCycle)*gcf);
	cleanUp("5");
}

int main() {
	/*

	*/

	for(;;){
		setLed(0);
		setLed(50);
		setLed(100);
	}
	return 0;
}
