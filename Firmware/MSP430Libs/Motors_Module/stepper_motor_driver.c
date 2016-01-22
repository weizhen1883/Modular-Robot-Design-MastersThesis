#include <msp430.h>
#include <stdlib.h>
#include "stepper_motor_driver.h"
#include "spi_slave.h"

#define WHEEL_DIFFERENCE 200 //wheel differences in mm
#define WHEEL_DIAMETER 85 //wheel diameter in mm 
#define WHEEL_PERIMETER 267 //wheel perimeter in mm
#define PI 3

volatile uint8_t DIRECTION_R = 0;
volatile uint8_t DIRECTION_L = 0;
volatile uint8_t MOTOR_SPEED_R = 0;
volatile uint8_t MOTOR_SPEED_L = 0;
volatile uint32_t CYCLES_COUNT_R = 0;
volatile uint32_t CYCLES_COUNT_L = 0;

/************ stepper_motor_init() ************
Enable the motor pin settings and Timer settings
***********************************************/
void stepper_motor_init(void) {
	//init the io pins for all stepper motors
	STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR1A) & (~STEPPERMOTOR_MR1B)
			& (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_ML1A)
			& (~STEPPERMOTOR_ML1B) & (~STEPPERMOTOR_ML2A)
			& (~STEPPERMOTOR_ML2B);
	STEPPERMOTOR_PORTDIR |= STEPPERMOTOR_MR1A | STEPPERMOTOR_MR1B
			| STEPPERMOTOR_MR2A | STEPPERMOTOR_MR2B | STEPPERMOTOR_ML1A
			| STEPPERMOTOR_ML1B | STEPPERMOTOR_ML2A | STEPPERMOTOR_ML2B;
	STEPPERMOTOR_PORTREN &= (~STEPPERMOTOR_MR1A) & (~STEPPERMOTOR_MR1B)
			& (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_ML1A)
			& (~STEPPERMOTOR_ML1B) & (~STEPPERMOTOR_ML2A)
			& (~STEPPERMOTOR_ML2B);

	//TIMER A CONFIG
	TA0CTL = TASSEL_2 | MC_0 | TACLR;
	TA1CTL = TASSEL_2 | MC_0 | TACLR;
	TA0CCTL0 &= ~CCIE;
	TA1CCTL0 &= ~CCIE;
}

/******* stepper_motor_direction_set() ********
Set the motor run direction
motor_sel: motor selection 0:right 1:left 2:all
direction: motor direction 0:front 1:back
***********************************************/
void stepper_motor_direction_set(uint8_t motor_sel, uint8_t direction) {
	switch (motor_sel) {
		case RIGHT_MOTOR:
			DIRECTION_R = direction;
			break;
		case LEFT_MOTOR:
			DIRECTION_L = direction;
			break;
		default:
			DIRECTION_R = direction;
			DIRECTION_L = direction;
			break;
	}
}

/**************** set_speed() *****************
set the motor speed
motor_sel: motor selection 0:right 1:left 2:all
speed: motor speed 5~50 rpm
***********************************************/
void set_speed(uint8_t motor_sel, uint8_t speed) {
	switch (motor_sel) {
		case RIGHT_MOTOR:
			MOTOR_SPEED_R = speed;
			break;
		case LEFT_MOTOR:
			MOTOR_SPEED_L = speed;
			break;
		default:
			MOTOR_SPEED_R = speed;
			MOTOR_SPEED_L = speed;
			break;
	}
}

/********** calculate_speed_cycles() **********
calculat and return the timer cycles by motor
speed in rpm
***********************************************/
uint16_t calculate_speed_cycles(uint8_t speed) {
	uint16_t steps = 0;
	uint16_t delay_cycles = 0;
	uint8_t i = 0;
	for (i = 0; i < speed; i++) {
		steps += 200;
	}
	delay_cycles = (uint16_t)(60000000 / steps);
	return delay_cycles;
}

/*************** timer_enable() ***************
enable the timmer interrupt
***********************************************/
void timer_enable(void) {
	TA0CCTL0 |= CCIE;
	TA0CCR0 = calculate_speed_cycles(MOTOR_SPEED_R);

	TA1CCTL0 |= CCIE;
	TA1CCR0 = calculate_speed_cycles(MOTOR_SPEED_L);

	TA0CTL = TASSEL_2 | MC_1 | TACLR;
	TA1CTL = TASSEL_2 | MC_1 | TACLR;
}

/*************** timer_disable() **************
disable the timmer interrupt
***********************************************/
void timer_disable(void) {
	TA0CCTL0 &= ~CCIE;
	TA1CCTL0 &= ~CCIE;

	TA0CTL = MC_0;
	TA1CTL = MC_0;
}

/**************** robot_go() ******************
the robot will go by different speed
speed_R, speed_L: motor speed 5~50 rpm
direction_R, direction_L: 0:front 1:back
***********************************************/
void robot_go(uint8_t speed_R, uint8_t direction_R, uint8_t speed_L, uint8_t direction_L) {
	CYCLES_COUNT_R = 0;
	CYCLES_COUNT_L = 0;
	stepper_motor_direction_set(RIGHT_MOTOR, direction_R);
	stepper_motor_direction_set(LEFT_MOTOR, direction_L);
	set_speed(RIGHT_MOTOR, speed_R);
	set_speed(LEFT_MOTOR, speed_L);
	timer_enable();
	return_message("M:RUNNING.");
}

/**************** robot_stop() ******************
the robot will stop
***********************************************/
void robot_stop(void) {
	timer_disable();
	set_speed(RIGHT_MOTOR, 0);
	set_speed(LEFT_MOTOR, 0);
	unsigned char *rightCount = (unsigned char*)&CYCLES_COUNT_R;
	unsigned char *leftCount = (unsigned char*)&CYCLES_COUNT_L;
	char buf[10] = {'M', ':', rightCount[0]+1, rightCount[1]+1, rightCount[2]+1, rightCount[3]+1, leftCount[0]+1, leftCount[1]+1, leftCount[2]+1, leftCount[3]+1};
	return_message((char *)buf);
}

/********** robot_get_cycle_count() ************
the robot will return the cycle count
***********************************************/
void robot_get_cycle_count(void) {
	unsigned char *rightCount = (unsigned char*)&CYCLES_COUNT_R;
	unsigned char *leftCount = (unsigned char*)&CYCLES_COUNT_L;
	char buf[10] = {'M', ':', rightCount[0]+1, rightCount[1]+1, rightCount[2]+1, rightCount[3]+1, leftCount[0]+1, leftCount[1]+1, leftCount[2]+1, leftCount[3]+1};
	return_message((char *)buf);
}

//TIMER A INTERRUPT A0 for right, A1 for left
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
	if (DIRECTION_R == 0) {
		switch(CYCLES_COUNT_R % 4) {
			case 0:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2A | STEPPERMOTOR_MR1B;
				break;
			case 1:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2A | STEPPERMOTOR_MR1A;
				break;
			case 2:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2B | STEPPERMOTOR_MR1A;
				break;
			case 3:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2B | STEPPERMOTOR_MR1B;
				break;
			default:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1B) & (~STEPPERMOTOR_MR1A);
				break;
		}
	} else {
		switch(CYCLES_COUNT_R % 4) {
			case 0:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2B | STEPPERMOTOR_MR1B;
				break;
			case 1:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2B | STEPPERMOTOR_MR1A;
				break;
			case 2:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2A | STEPPERMOTOR_MR1A;
				break;
			case 3:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_MR2A | STEPPERMOTOR_MR1B;
				break;
			default:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_MR2B) & (~STEPPERMOTOR_MR2A) & (~STEPPERMOTOR_MR1B) & (~STEPPERMOTOR_MR1A);
				break;
		}
	}
	CYCLES_COUNT_R++;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1(void) {
	if (DIRECTION_L == 0) {
		switch(CYCLES_COUNT_L % 4) {
			case 0:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2B | STEPPERMOTOR_ML1B;
				break;
			case 1:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2B | STEPPERMOTOR_ML1A;
				break;
			case 2:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2A | STEPPERMOTOR_ML1A;
				break;
			case 3:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2A | STEPPERMOTOR_ML1B;
				break;
			default:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1B) & (~STEPPERMOTOR_ML1A);
				break;
		}
	} else {
		switch(CYCLES_COUNT_L % 4) {
			case 0:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2A | STEPPERMOTOR_ML1B;
				break;
			case 1:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2A | STEPPERMOTOR_ML1A;
				break;
			case 2:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1B);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2B | STEPPERMOTOR_ML1A;
				break;
			case 3:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1A);
				STEPPERMOTOR_PORTOUT |= STEPPERMOTOR_ML2B | STEPPERMOTOR_ML1B;
				break;
			default:
				STEPPERMOTOR_PORTOUT &= (~STEPPERMOTOR_ML2B) & (~STEPPERMOTOR_ML2A) & (~STEPPERMOTOR_ML1B) & (~STEPPERMOTOR_ML1A);
				break;
		}
	}
	CYCLES_COUNT_L++;
}