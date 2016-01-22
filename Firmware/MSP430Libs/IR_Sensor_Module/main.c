#include <msp430g2553.h>
#include <stdint.h>
#include "adc.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHzF

	ir_sensor_init();

	__bis_SR_register(GIE + LPM0_bits); //Interrupts Enabled

	return 0;
}