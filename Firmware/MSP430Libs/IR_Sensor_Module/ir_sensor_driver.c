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