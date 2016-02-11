#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stepper_motor_api.h"
#include "ir_sensor_api.h"

int main(int argc, char const *argv[])
{
	robotMotorModuleInit(17);
	IRSensorModuleInit(4);
	printf("%s\n", argv[1]);
	if (!strcmp(argv[1],"go")) {
		if (argc > 3) {
			uint8_t speed_r = atoi(argv[2]);
			uint8_t direction_r = atoi(argv[3]);
			uint8_t speed_l = atoi(argv[4]);
			uint8_t direction_l = atoi(argv[5]);
			printf("%d %d %d %d\n",speed_r,direction_r,speed_l,direction_l);
			robotGo(speed_r,direction_r,speed_l,direction_l);
		} else {
			robotGo(50,0,50,0);
		}
	} else if (!strcmp(argv[1],"stop")) {
		robotStop();
	} else if (!strcmp(argv[1], "get")) {
		robotGet();
	} else if (!strcmp(argv[1], "cget")) {
		robotCGet();
	} else if (!strcmp(argv[1], "irget")) {
		if (argc > 2) {
		 	printf("IR Sensor %s: %d\n", argv[2], get_ir_data(argv[2]));
		} else {
			uint16_t data[4];
			get_ir_datas(data);
			printf("F:%d B:%d L:%d R:%d\n", data[0], data[1], data[2], data[3]);
		}
		
	}
	return 0;
}