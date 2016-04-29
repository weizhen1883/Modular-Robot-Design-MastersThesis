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

uint8_t Kp = 2;
struct Motors motors;

uint8_t LeftWallFollowing(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t states);
void P_Controller(struct SensorLimits IRLimits, uint16_t* sensorData, uint8_t mode);

int main(int argc, char const *argv[])
{
	struct SensorLimits IRLimits;
	IRLimits.frontLimit = Calculate_Front_IR_Value(10);
	IRLimits.leftLimit = Calculate_Left_IR_Value(5);
	IRLimits.rightLimit = Calculate_Right_IR_Value(5);
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

	robotMotorModuleInit(4);
	IRSensorModuleInit(17);

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
			} else if (sensorData[2] < Calculate_Left_IR_Value(20)) {
				robotStop();
				for (i = 0; i < 50000; i++);
				nextStates = 4;
			} else {
				P_Controller(IRLimits, sensorData, MODE_FOLLOW_LEFT_WALL);
				robotGo(motors.speed_r, motors.direction_r, motors.speed_l, motors.direction_l);
				for (i = 0; i < 50000; i++);
				nextStates = 1;
			}
			break;
		case 2:
			robotGo(30,0,30,1);
			for (i = 0; i < 50000; i++);
			nextStates = 3;
			break;
		case 3:
			if (sensorData[0] < IRLimits.frontLimit && sensorData[2] > Calculate_Left_IR_Value(9)) {
				robotStop();
				for (i = 0; i < 100000; i++);
				nextStates = 0;
				motors.speed_l = 30;
				motors.speed_r = 30;
				motors.direction_l = 1;
				motors.direction_r = 1;
			} else {
				nextStates = 3;
			}
			break;
		case 4:
			robotGo(50,1,15,1);
			for (i = 0; i < 50000; i++);
			nextStates = 5;
			break;
		case 5:
			if (sensorData[0] > Calculate_Left_IR_Value(8)) {
				robotStop();
				for (i = 0; i < 50000; i++);
				nextStates = 6;	
			} else {
				nextStates = 5;
			}
			break;
		case 6:
			robotGo(30,0,30,1);
			for (i = 0; i < 50000; i++);
			nextStates = 7;
			break;
		case 7:
			if (sensorData[0] < IRLimits.frontLimit && sensorData[2] > Calculate_Left_IR_Value(5)) {
				robotStop();
				for (i = 0; i < 100000; i++);
				nextStates = 1;
				motors.speed_l = 30;
				motors.speed_r = 30;
				motors.direction_l = 1;
				motors.direction_r = 1;
			} else {
				nextStates = 7;
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
	int cur_error;
	double cur_error_inch;
	switch (mode) {
		case MODE_FOLLOW_LEFT_WALL:
			cur_error = IRLimits.leftLimit - sensorData[2];
			cur_error_inch = Calculate_Left_Inch_Value((double)sensorData[2]) - 10.;
			motors.speed_l = (uint8_t)(30. - cur_error_inch * ((double)Kp)); 
			motors.speed_r = 30;
			printf("the error is %d, and %f inch\n", cur_error, cur_error_inch);
			printf("the left speed is %d\n", motors.speed_l);
			printf("the right speed is %d\n", motors.speed_r);
			break;
	}
}

