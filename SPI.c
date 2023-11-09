#include "lpc214x.h"
#include "stdio.h"
#include "SPI.h"

void delay(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
		for(j=0;j<3000;j++);
}

void spi0_init(void)
{
	PINSEL0 = PINSEL0 & 0xFFFF00FF;
	PINSEL0 = PINSEL0 | 0x00001500;					//SCK0, MISO, MOSI configured

	IODIR0 |= SSEL0;					  			//Slave Select as output
	S0SPCCR = 0x10;									//Set clock
	IOSET0 	= SSEL0;								// CS is high							 
	S0SPCR 	= 0x0020;								//configure in SPI master mode, 8 bit
}

unsigned char spi0_write(unsigned char ch)
{
	S0SPDR = ch;   									//write to SPI data register
	while(!(S0SPSR & SPIF));					   	//wait for SPIF flag to set (data transmitted)
	ch = S0SPDR;								   	//read SPI data register
	return ch;
} 

unsigned char spi0_read()
{
	unsigned char ch;
	S0SPDR = 0xFF;								 	//write dummy data to generate clock pulses
	while(!(S0SPSR & SPIF));					   	//wait for SPIF flag to be set (data received)
	ch = S0SPDR;								   	//read SPI data register and return
	return ch;
}
