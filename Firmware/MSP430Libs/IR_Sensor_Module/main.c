#include <msp430g2553.h>
#include <stdint.h>
#include "ir_sensor_driver.h"
#include "spi_slave.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHzF

	ir_sensor_init();
	spi_init();
	return_message("S:IR_READY");

	__bis_SR_register(LPM0_bits + GIE); //Interrupts Enabled

	return 0;
}