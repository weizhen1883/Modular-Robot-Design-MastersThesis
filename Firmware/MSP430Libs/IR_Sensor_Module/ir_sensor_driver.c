#include <stdint.h>
#include "ir_sensor_driver.h"
#include "adc.h"
#include "spi_slave.h"

void ir_sensor_init(void) {
	init_adc();
}

uint16_t get_IR_sensor_data(uint8_t IR_number) {
	return adc_convert(IR_number);
}

void prepare_IR_data(void) {
	uint16_t sensorData_F = get_IR_sensor_data(0);
	uint16_t sensorData_B = get_IR_sensor_data(2);
	uint16_t sensorData_L = get_IR_sensor_data(3);
	uint16_t sensorData_R = get_IR_sensor_data(1);

	unsigned char data_F_0 = (unsigned char)((sensorData_F & 0xFF00) >> 8) + 1;
	unsigned char data_F_1 = (unsigned char)(sensorData_F & 0x00FF);
	if (data_F_1 == 0x00) data_F_1 = 0x01;

	unsigned char data_B_0 = (unsigned char)((sensorData_B & 0xFF00) >> 8) + 1;
	unsigned char data_B_1 = (unsigned char)(sensorData_B & 0x00FF);
	if (data_B_1 == 0x00) data_B_1 = 0x01;

	unsigned char data_L_0 = (unsigned char)((sensorData_L & 0xFF00) >> 8) + 1;
	unsigned char data_L_1 = (unsigned char)(sensorData_L & 0x00FF);
	if (data_L_1 == 0x00) data_L_1 = 0x01;

	unsigned char data_R_0 = (unsigned char)((sensorData_R & 0xFF00) >> 8) + 1;
	unsigned char data_R_1 = (unsigned char)(sensorData_R & 0x00FF);
	if (data_R_1 == 0x00) data_R_1 = 0x01;

	char buf[10] = {'S', ':', data_F_0, data_F_1, data_B_0, data_B_1, data_L_0, data_L_1, data_R_0, data_R_1};
	return_message((char *)buf);
}

void get_IR_data(void) {
	return_message("S:IR_READY");
}