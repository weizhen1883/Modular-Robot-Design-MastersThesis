#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gpio.h"
#include "spi.h"

#include "stepper_motor_api.h"
#include "ir_sensor_api.h"
#include "ir_functions.h"

uint8_t ObstacleAvoid(uint16_t frontLimit, uint16_t* sensorData, uint8_t states);

int main(int argc, char const *argv[])
{
	uint16_t frontLimit = Calculate_Front_IR_Value(10);
	uint16_t sensorData[4];
	int i;
	uint8_t states = 0;

	printf("Front Limit is %d\n", frontLimit);

	robotMotorModuleInit(4);
	IRSensorModuleInit(17);
	printf("Obstacle Avoidance project start!\n");

	while(1) {
		get_ir_datas(sensorData);
		for(i = 0; i < 50000; i++);
		states = ObstacleAvoid(frontLimit, sensorData, states);
	}

	return 0;
}

uint8_t ObstacleAvoid(uint16_t frontLimit, uint16_t* sensorData, uint8_t states) {
	int i;
	uint8_t nextStates = states;

	switch (states) {
		case 0:
			robotGo(30,1,30,1);
			for (i = 0; i < 50000; i++);
			nextStates = 1;
			break;
		case 1:
			if (sensorData[0] > frontLimit) {
				robotStop();
				for (i = 0; i < 50000; i++);
				nextStates = 2;
			}
			break;
		case 2:
			robotGo(30,0,30,1);//turn right
			for (i = 0; i < 50000; i++);
			nextStates = 3;
			break;
		case 3:
			if (sensorData[0] < frontLimit) {
				robotStop();
				for (i = 0; i < 50000; i++);
				nextStates = 0;
			}
			break;
		default:
			robotStop();
			for (i = 0; i < 50000; i++);
			break;
	}

	return nextStates;
}

