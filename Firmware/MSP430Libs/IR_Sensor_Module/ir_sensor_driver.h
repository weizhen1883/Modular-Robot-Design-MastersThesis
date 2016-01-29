#ifndef _IR_SENSOR_DRIVER_H_
#define _IR_SENSOR_DRIVER_H_

#define IR_SENSOR_F 0
#define IR_SENSOR_B 1
#define IR_SENSOR_L 2
#define IR_SENSOR_R 3

void ir_sensor_init(void);
uint16_t get_IR_sensor_data(uint8_t IR_number);
void prepare_IR_data(void);
void get_IR_data(void);

#endif