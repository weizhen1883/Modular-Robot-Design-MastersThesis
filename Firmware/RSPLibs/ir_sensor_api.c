#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "gpio.h"
#include "spi.h"

#include "ir_sensor_api.h"

uint8_t IR_Sensor_Module_Sel_Pin = 24;

void IRSensorModuleInit(uint8_t en_pin) {
	IR_Sensor_Module_Sel_Pin = en_pin;
	printf("The IR Sensor Module Connected to Pin%d\n", en_pin);
}

void preIR(void) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'S', ':', 'P', 'R', 'E', ' ', 'I', 'R', ' ', '\n'};

	if (gpio_open(&spi_sel, IR_Sensor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
		fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	if (gpio_write(&spi_sel, !value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	if (spi_open(&spi, "/dev/spidev0.0",  0, 10000) < 0) {
		fprintf(stderr, "spi_open(): %s\n", spi_errmsg(&spi));
		exit(1);
	}

	if (spi_transfer(&spi, buf, buf, sizeof(buf)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(&spi));
		exit(1);
	}
	printf("shifted in: %s\n", buf);

	spi_close(&spi);

	if (gpio_write(&spi_sel, value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	gpio_close(&spi_sel);
}

void getIR(uint16_t* data) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'S', ':', 'G', 'E', 'T', ' ', 'I', 'R', ' ', '\n'};

	if (gpio_open(&spi_sel, IR_Sensor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
		fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	if (gpio_write(&spi_sel, !value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	if (spi_open(&spi, "/dev/spidev0.0",  0, 10000) < 0) {
		fprintf(stderr, "spi_open(): %s\n", spi_errmsg(&spi));
		exit(1);
	}

	if (spi_transfer(&spi, buf, buf, sizeof(buf)) < 0) {
		fprintf(stderr, "spi_transfer(): %s\n", spi_errmsg(&spi));
		exit(1);
	}

	uint16_t dataFront = ((uint16_t)(buf[2]-1) << 8) | (buf[3]);
	uint16_t dataBack = ((uint16_t)(buf[4]-1) << 8) | (buf[5]);
	uint16_t dataLeft = ((uint16_t)(buf[6]-1) << 8) | (buf[7]);
	uint16_t dataRight = ((uint16_t)(buf[8]-1) << 8) | (buf[9]);
	printf("shifted in: %c%cF=%d,B=%d,L=%d,R=%d\n", buf[0], buf[1], dataFront, dataBack, dataLeft, dataRight);

	data[0] = dataFront;
	data[1] = dataBack;
	data[2] = dataLeft;
	data[3] = dataRight;

	spi_close(&spi);

	if (gpio_write(&spi_sel, value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	gpio_close(&spi_sel);
}

void get_ir_datas(uint16_t* data) {
	preIR();
	//delay
	int i;
	for (i = 0; i < 15000; i++);
	getIR(data);
}

uint16_t get_ir_data(uint8_t* IR_Sensor) {
	uint16_t data[4];
	get_ir_datas(data);
	if (!strcmp(IR_Sensor,"front")) {
		return data[0];
	} else if (!strcmp(IR_Sensor,"back")) {
		return data[1];
	} else if (!strcmp(IR_Sensor,"left")) {
		return data[2];
	} else if (!strcmp(IR_Sensor,"right")) {
		return data[3];
	} else {
		return 0;
	}
}