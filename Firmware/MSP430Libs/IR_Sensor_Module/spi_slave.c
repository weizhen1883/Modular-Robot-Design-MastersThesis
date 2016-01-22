#include <msp430g2553.h>
#include <string.h>
#include "spi_slave.h"
#include "ir_sensor_driver.h"

char cmdbuf[20];
char cmd_index=0;

void spi_init(void) {
	P1SEL |= BIT4 | BIT5 | BIT6 | BIT7;
    P1SEL2 |= BIT4 | BIT5 | BIT6 | BIT7;
    UCB0CTL1 = UCSWRST; 
    UCB0CTL0 |= UCMSB + UCSYNC + UCCKPH + UCMODE_1;
    UCB0CTL1 &= ~UCSWRST;
    IE2 |= UCB0RXIE;
}

void spi_putc(unsigned char c) {
	while (!(IFG2 & UCB0TXIFG));
	UCB0TXBUF = c;
}

void return_message(char *buf) {
	int i = 0;
	while (*buf) {
		txbuf[i] = *buf++;
		i++;
	}
	spi_putc(txbuf[0]);
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	char rx_char;

	while (IFG2 & UCB0RXIFG) {
		rx_char = UCB0RXBUF;
		if (rx_char == '\n') {
			if (strncmp(cmdbuf, "M:GO", 4) == 0) {
				robot_go(cmdbuf[4],cmdbuf[5]-'0',cmdbuf[6],cmdbuf[7]-'0');
			} 
			cmd_index = 0;
			//spi_putc(txbuf[cmd_index]);
		} else {
			cmdbuf[cmd_index] = rx_char;
			cmd_index++;
			spi_putc(txbuf[cmd_index]);
		}
	}
}