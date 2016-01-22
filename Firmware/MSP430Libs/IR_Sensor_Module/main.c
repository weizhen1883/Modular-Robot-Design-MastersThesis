#include <msp430g2553.h>
#include <stdint.h>
#include "ir_sensor_init.h"
#include "spi_slave.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHzF

	ir_sensor_init();
	spi_init();
	return_message("RobotReady");

	__bis_SR_register(GIE); //Interrupts Enabled

	while(1) {
		
	}

	return 0;
}