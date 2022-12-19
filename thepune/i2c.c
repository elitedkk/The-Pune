#include "bmp180.c"
#include <unistd.h>
#include <stdio.h>

//int main(int argc, char **argv){
void getBMPValues(float *avg_temp, float *avg_p, float *avg_alt){
	char *i2c_device = "/dev/i2c-1";
	int address = 0x77;
	
	
//	while(1){
	void *bmp = bmp180_init(address, i2c_device);
	if(bmp != NULL){
		int i;
		int readNum = 5;
		for(i = 0; i < readNum; i++) {
			usleep(2 * 10 * 1000);
			float t = bmp180_temperature(bmp);
			long p = bmp180_pressure(bmp);
			float alt = bmp180_altitude(bmp);

			*avg_temp 	+= t/readNum;
			*avg_p 		+= p/readNum;
			*avg_alt	+= alt/readNum;
			//printf("t = %f, p = %lu, a = %f\n", t, p, alt);
			usleep(2 * 10 * 1000);
		}
		/*
		printf("Average temp = %f\n", *avg_temp);
		printf("Average pressure = %lu\n", *avg_p);
		printf("Average altitude = %f\n", *avg_alt);
		*/
		bmp180_close(bmp);
	}
//	}
	return 0;
}
