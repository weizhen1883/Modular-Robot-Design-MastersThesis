#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "gpio.h"
#include "spi.h"

#include "stepper_motor_api.h"

uint8_t Motor_Module_Sel_Pin = 23;

void robotMotorModuleInit(uint8_t en_pin) {
	Motor_Module_Sel_Pin = en_pin;
	printf("The Motor Module Connected to Pin%d\n", en_pin);
}

void robotGo(uint8_t speed_r, uint8_t direction_r, uint8_t speed_l, uint8_t direction_l) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'M', ':', 'G', 'O', speed_r, '0'+direction_r, speed_l, '0'+direction_l, ' ', '\n'};
	printf("shifted out: %s\n", buf);

	if (gpio_open(&spi_sel, Motor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
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

void robotStop(void) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'M', ':', 'S', 'T', 'O', 'P', ' ', ' ', ' ', '\n'};
	printf("shifted out: %s\n", buf);

	if (gpio_open(&spi_sel, Motor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
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

void robotGet(void) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'M', ':', 'G', 'E', 'T', ' ', ' ', ' ', ' ', '\n'};
	printf("shifted out: %s\n", buf);

	if (gpio_open(&spi_sel, Motor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
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

	uint32_t rightCount = ((uint32_t)(buf[5]-1) << 24) | ((buf[4]-1) << 16) | ((buf[3]-1) << 8) | (buf[2]-1);
	uint32_t leftCount = ((uint32_t)(buf[9]-1) << 24) | ((buf[8]-1) << 16) | ((buf[7]-1) << 8) | (buf[6]-1);
	printf("shifted in: %c%cL=%ld,R=%ld\n", buf[0], buf[1], leftCount, rightCount);

	spi_close(&spi);

	if (gpio_write(&spi_sel, value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	gpio_close(&spi_sel);
}

void robotCGet(void) {
	gpio_t spi_sel;
	spi_t spi;
	bool value;
	uint8_t buf[10] = {'M', ':', 'C', 'G', 'E', 'T', ' ', ' ', ' ', '\n'};
	printf("shifted out: %s\n", buf);

	if (gpio_open(&spi_sel, Motor_Module_Sel_Pin, GPIO_DIR_OUT) < 0) {
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

	uint32_t leftCount = ((uint32_t)(buf[5]-1) << 24) | ((buf[4]-1) << 16) | ((buf[3]-1) << 8) | (buf[2]-1);
	uint32_t rightCount = ((uint32_t)(buf[9]-1) << 24) | ((buf[8]-1) << 16) | ((buf[7]-1) << 8) | (buf[6]-1);
	printf("shifted in: %c%cL=%ld,R=%ld\n", buf[0], buf[1], leftCount, rightCount);

	spi_close(&spi);

	if (gpio_write(&spi_sel, value) < 0) {
		fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(&spi_sel));
		exit(1);
	}

	gpio_close(&spi_sel);
}