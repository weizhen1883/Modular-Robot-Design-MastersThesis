#ifndef _STEPPER_MOTOR_API_H
#define _STEPPER_MOTOR_API_H

void robotMotorModuleInit(uint8_t en_pin);
void robotGo(uint8_t speed_r, uint8_t direction_r, uint8_t speed_l, uint8_t direction_l);
void robotStop(void);
void robotGet(void);
void robotCGet(void);

#endif