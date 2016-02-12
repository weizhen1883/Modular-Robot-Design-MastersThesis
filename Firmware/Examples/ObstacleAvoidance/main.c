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

	// gpio_t spi_sel;
	// spi_t spi;
	// bool value;
	// uint8_t buf[10] = {'M', ':', 'S', 'T', 'O', 'P', ' ', ' ', ' ', '\n'};
	// printf("shifted out: %s\n", buf);

	// if (gpio_open(&spi_sel, 17, GPIO_DIR_OUT) < 0) {
	// 	fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(&spi_sel));
	// 	exit(1);
	// }

	// if (gpio_write(&spi_sel, !value) < 0) {
	// 	fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
	// 	exit(1);
	// }

	// if (spi_open(&spi, "/dev/spidev0.0",  0, 10000) < 0) {
	// 	fprintf(stderr, "spi_open(): %s\n", spi_errmsg(&spi));
	// 	exit(1);
	// }

	// if (spi_transfer(&spi, buf, buf, sizeof(buf)) < 0) {
	// 	fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(&spi));
	// 	exit(1);
	// }

	// printf("shifted in: %s\n", buf);

	// spi_close(&spi);

	// if (gpio_write(&spi_sel, value) < 0) {
	// 	fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
	// 	exit(1);
	// }

	// gpio_close(&spi_sel);

	// printf("%s\n", argv[1]);
	// if (!strcmp(argv[1],"go")) {
	// 	if (argc > 3) {
	// 		uint8_t speed_r = atoi(argv[2]);
	// 		uint8_t direction_r = atoi(argv[3]);
	// 		uint8_t speed_l = atoi(argv[4]);
	// 		uint8_t direction_l = atoi(argv[5]);
	// 		printf("%d %d %d %d\n",speed_r,direction_r,speed_l,direction_l);
	// 		robotGo(speed_r,direction_r,speed_l,direction_l);
	// 	} else {
	// 		robotGo(50,0,50,0);
	// 	}
	// } else if (!strcmp(argv[1],"stop")) {
	// 	robotStop();
	// } else if (!strcmp(argv[1], "get")) {
	// 	robotGet();
	// } else if (!strcmp(argv[1], "cget")) {
	// 	robotCGet();
	// } else if (!strcmp(argv[1], "irget")) {
	// 	if (argc > 2) {
	// 	 	printf("IR Sensor %s: %d\n", argv[2], get_ir_data(argv[2]));
	// 	} else {
	// 		uint16_t data[4];
	// 		get_ir_datas(data);
	// 		printf("F:%d B:%d L:%d R:%d\n", data[0], data[1], data[2], data[3]);
	// 	}
		
	// }
	return 0;
}