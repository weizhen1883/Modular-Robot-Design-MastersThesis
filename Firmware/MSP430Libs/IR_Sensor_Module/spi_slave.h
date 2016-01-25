#ifndef _SPI_H_
#define _SPI_H_

char txbuf[20];

uint16_t sensorData_F;
uint16_t sensorData_B;
uint16_t sensorData_L;
uint16_t sensorData_R;

void spi_init(void);
void spi_putc(unsigned char c);
void return_message(char *buf);

#endif