#ifndef SPI_H
#define SPI_H
					 
#define SSEL0 1<<7
#define SPIF  1<<7

void spi0_init(void);
unsigned char spi0_write(unsigned char);
unsigned char spi0_read(void);
void delay(unsigned int);

#endif
