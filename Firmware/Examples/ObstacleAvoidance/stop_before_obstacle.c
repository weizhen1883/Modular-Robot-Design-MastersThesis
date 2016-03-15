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
	uint16_t sensorData[4];
	int i;

	printf("Front Limit is %d\n", frontLimit);

	robotMotorModuleInit(17);
	IRSensorModuleInit(4);
	printf("Obstacle Avoidance project start!\n");

	robotGo(20,1,20,1);

	for (i = 0; i < 50000; i++);
	get_ir_datas(sensorData);
	while (sensorData[0] < frontLimit) {
		for (i = 0; i < 50000; i++);
		get_ir_datas(sensorData);
	}
	for (i = 0; i < 50000; i++);
	robotStop();
	for (i = 0; i < 50000; i++);
	robotGet();

	return 0;
}