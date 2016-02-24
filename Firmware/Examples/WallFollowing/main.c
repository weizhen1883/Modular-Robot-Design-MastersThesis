#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "gpio.h"
#include "spi.h"

#include "stepper_motor_api.h"
#include "ir_sensor_api.h"
#include "ir_functions.h"

#include "pi_robot_headers.h"

#define MODE_FOLLOW_LEFT_WALL 0
#define MODE_FOLLOW_RIGHT_WALL 1
#define MODE_IN_THE_MIDDLE_OF_WALLS 2

uint8_t Kp = 1;
struct Motors motors;

uint8_t LeftWallFollowing(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t states);
void P_Controller(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t mode);

int main(int argc, char const *argv[])
{
	struct SensorLimits IRLimits;
	IRLimits.frontLimit = Calculate_Front_IR_Value(10);
	IRLimits.leftLimit = Calculate_Left_IR_Value(10);
	IRLimits.rightLimit = Calculate_Right_IR_Value(10);
	uint16_t sensorData[4];
	int i;
	uint8_t states = 0;

	printf("Front Limit is %d\n", IRLimits.frontLimit);
	printf("Left Limit is %d\n", IRLimits.leftLimit);
	printf("Right Limit is %d\n", IRLimits.rightLimit);

	motors.speed_l = 30;
	motors.speed_r = 30;
	motors.direction_l = 1;
	motors.direction_r = 1;

	robotMotorModuleInit(17);
	IRSensorModuleInit(4);
	printf("Obstacle Avoidance project start!\n");

	while(1) {
		get_ir_datas(sensorData);
		for(i = 0; i < 50000; i++);
		states = LeftWallFollowing(IRLimits, sensorData, states);
	}

	return 0;
}

uint8_t LeftWallFollowing(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t states) {
	int i;
	uint8_t nextStates = states;

	switch (states) {
		case 0:
			robotGo(motors.speed_r, motors.direction_r, motors.speed_l, motors.direction_l);
			for (i = 0; i < 50000; i++);
			nextStates = 1;
			break;
		case 1:
			if (sensorData[0] > IRLimits.frontLimit) {
				robotStop();
				for (i = 0; i < 50000; i++);
				nextStates = 2;
			} else {
				P_Controller(IRLimits, sensorData, MODE_FOLLOW_LEFT_WALL);
				for (i = 0; i < 5000; i++);
				robotGo(motors.speed_r, motors.direction_r, motors.speed_l, motors.direction_l);
				for (i = 0; i < 50000; i++);
				nextStates = 1;
			}
			break;
		case 2:
			robotGo(30,0,30,1);//turn right
			for (i = 0; i < 50000; i++);
			nextStates = 3;
			break;
		case 3:
			if (sensorData[0] < IRLimits.frontLimit) {
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

void P_Controller(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t mode) {
	uint8_t tmp_speed_l;
	switch (mode) {
		case MODE_FOLLOW_LEFT_WALL:
			if (sensorData[2] > IRLimits.leftLimit) {
				tmp_speed_l = motors.speed_l + Kp * (sensorData[2] - IRLimits.leftLimit) / 100;
				if (tmp_speed_l < 50) {
					motors.speed_l = tmp_speed_l;
					motors.direction_l = 1;
					motors.direction_r = 1;
				} else {
					motors.speed_l = 30;
					motors.direction_l = 1;
					motors.direction_r = 0;
				}
				
			} else {
				tmp_speed_l = motors.speed_l - Kp * (IRLimits.leftLimit - sensorData[2]) / 100;
				if (tmp_speed_l > 0) {
					motors.speed_l = tmp_speed_l;
					motors.direction_l = 1;
					motors.direction_r = 1;
				} else {
					motors.speed_l = 30;
					motors.direction_l = 0;
					motors.direction_r = 1;
				}
			}
			break;
		// case MODE_FOLLOW_RIGHT_WALL:
		// 	cur_error = IRLimits.rightLimit - sensorData[3];
		// 	motors.speed_r = motors.speed_r - cur_error * Kp; 
		// 	break;
		// case MODE_IN_THE_MIDDLE_OF_WALLS:
		// 	cur_error = IRLimits.leftLimit - sensorData[2];
		// 	motors.speed_l = motors.speed_l - cur_error * Kp;
		// 	cur_error = IRLimits.rightLimit - sensorData[3];
		// 	motors.speed_r = motors.speed_r - cur_error * Kp; 
		// 	break;
	}
}

