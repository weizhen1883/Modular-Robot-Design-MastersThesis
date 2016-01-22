/* stepper_motor_driver.h
 * For 28Pin MSP430G2553
 *
 *
 *		write by Zhen
 */
#ifndef _STEPPER_MOTOR_DRIVER_H
#define _STEPPER_MOTOR_DRIVER_H

#include <stdint.h>

/* Define step motor pin */
#define STEPPERMOTOR_PORTOUT P3OUT
#define STEPPERMOTOR_PORTDIR P3DIR
#define STEPPERMOTOR_PORTREN P3REN
//define the right motor pin
#define STEPPERMOTOR_MR1A BIT0
#define STEPPERMOTOR_MR1B BIT1
#define STEPPERMOTOR_MR2A BIT2
#define STEPPERMOTOR_MR2B BIT3
//define the left motor pin
#define STEPPERMOTOR_ML1A BIT4
#define STEPPERMOTOR_ML1B BIT5
#define STEPPERMOTOR_ML2A BIT6
#define STEPPERMOTOR_ML2B BIT7

//define some constant values
#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1
#define BOTH_MOTOR 2
#define DIRECTION_FORWARD 0
#define DIRECTION_BACKWARD 1
#define DIRECTION_CLOCIWISE 0
#define DIRECTION_COUNTERCLOCKWISE 1


void stepper_motor_init(void);
void robot_go(uint8_t speed_R, uint8_t direction_R, uint8_t speed_L, uint8_t direction_L);
void robot_stop(void);
void robot_go_forward(uint8_t speed);
void robot_go_backward(uint8_t speed);
void robot_go_round(uint16_t turn_radius, uint8_t speed, uint8_t direction);

#endif
