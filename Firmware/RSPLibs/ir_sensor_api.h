#ifndef _STEPPER_MOTOR_API_H
#define _STEPPER_MOTOR_API_H

void IRSensorModuleInit(uint8_t en_pin);
void getIR(uint16_t* data);
void preIR(void);
void get_ir_datas(uint16_t* data);
uint16_t get_ir_data(uint8_t* IR_Sensor);

#endif