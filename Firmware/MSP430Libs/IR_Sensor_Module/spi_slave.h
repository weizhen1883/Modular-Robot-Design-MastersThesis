#ifndef _SPI_H_
#define _SPI_H_

char txbuf[10];

void spi_init(void);
void spi_putc(unsigned char c);
void return_message(char *buf);

#endif