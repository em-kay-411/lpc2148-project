#include "lpc214x.h"
#include "stdio.h"
void UartInit(unsigned int baudrate)	  //setting the baud rate for 115200 baud
{
	unsigned int FDiv;
	
	PINSEL0 = PINSEL0 & 0xFFFFFFF0;	   // modify the value
	PINSEL0 = PINSEL0 | 0x00000005;	   // set the functionality of the TxD and Rxd Pin	:01
	
	//set the baud rate
	U0LCR = 0x83;						// Line control register :DLAB=1 ; 8 bits ; 1 stop bit ; no parity
	FDiv = (15000000 / 16 ) / baudrate ; //
	U0DLM = FDiv /256;					//0x00;
	U0DLL = FDiv %256;					//0x97;
	U0LCR = 0x03;						// Line control register :DLAB=0 ; 8 bits ; 1 stop bit ; no parity
	U0TER = 0x80;
}

int UART_GetChar(void)
{
	while(!(U0LSR & 0x1));
	return(U0RBR);
}

int UART_PutChar(unsigned char Ch)
{
  	while(!(U0LSR & 0x20));
	return( U0THR = Ch);
}


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
  	return (UART_PutChar(ch));
}
