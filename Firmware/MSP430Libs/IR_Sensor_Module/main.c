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
	return_message("RobotReady");

	__bis_SR_register(GIE); //Interrupts Enabled

	// uint16_t sensorData_F;
	// uint16_t sensorData_B;
	// uint16_t sensorData_L;
	// uint16_t sensorData_R;
	unsigned char *frontSensorDate;
	unsigned char *backSensorDate;
	unsigned char *leftSensorDate;
	unsigned char *rightSensorDate;

	while(1) {
		sensorData_F = get_IR_sensor_data(IR_SENSOR_F);
		sensorData_B = get_IR_sensor_data(IR_SENSOR_B);
		sensorData_L = get_IR_sensor_data(IR_SENSOR_L);
		sensorData_R = get_IR_sensor_data(IR_SENSOR_R);
		frontSensorDate = (unsigned char *)&sensorData_F;
		backSensorDate = (unsigned char *)&sensorData_B;
		leftSensorDate = (unsigned char *)&sensorData_L;
		rightSensorDate = (unsigned char *)&sensorData_R;
		char buf[10] = {'S', ':', frontSensorDate[1], frontSensorDate[0], backSensorDate[1], backSensorDate[0], 
			leftSensorDate[1], leftSensorDate[0], rightSensorDate[1], rightSensorDate[0]};
		return_message((char *)buf);
	}

	return 0;
}