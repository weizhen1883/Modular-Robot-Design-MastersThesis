/* main.c
 * Motor Module main function
 *
 *	write by Zhen
 */
#include <msp430.h>
#include "stepper_motor_driver.h"
#include "spi_slave.h"

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHz

	stepper_motor_init();
	spi_init();
	//robot_go_forward(50);
	return_message("RobotReady");

	__bis_SR_register(LPM0_bits + GIE); //Interrupts Enabled and Low Power Mode

	return 0;
}
