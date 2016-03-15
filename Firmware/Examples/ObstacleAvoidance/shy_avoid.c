#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gpio.h"
#include "spi.h"

#include "stepper_motor_api.h"
#include "ir_sensor_api.h"
#include "ir_functions.h"

int main(int argc, char const *argv[])
{
	uint16_t frontLimit = Calculate_Front_IR_Value(10);
	uint16_t backLimit = Calculate_Back_IR_Value(10);
	uint16_t sensorData[4];
	int i;
	uint8_t status = 0;

	printf("Front Limit is %d\n", frontLimit);
	printf("Back Limit is %d\n", backLimit);

	robotMotorModuleInit(17);
	IRSensorModuleInit(4);
	printf("Obstacle Avoidance project start!\n");


	while(1) {
		get_ir_datas(sensorData);
		for (i = 0; i < 50000; i++);
		if (sensorData[0] > frontLimit && sensorData[1] < backLimit) {
			if (status != 'F') {
				robotGo(20,0,20,0);
				for (i = 0; i < 50000; i++);
				status = 'F';
			}
		} else if (sensorData[0] < frontLimit && sensorData[1] > backLimit) {
			if (status != 'B') {
				robotGo(20,1,20,1);
				for (i = 0; i < 50000; i++);
				status = 'B';
			}
		} else {
			robotStop();
			for (i = 0; i < 50000; i++);
		}
	}

	return 0;
}