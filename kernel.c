//#include "kernel.h"
#include "lpc214x.h"
#include "uart.h"
#include "stdio.h"

/*
void kernelHexData(void){
//   	printf("\r\nKernel loaded successfully....");
//   	printf("\r\nHit any Key on your PC Keyboard to see input echoing\r\n\n\n");
	
	while(1)
	{
		while(!(U0LSR & 0x1));
		Ch = U0RBR;
		if (Ch == '\n')  {
    while (!(U0LSR & 0x20));
    U0THR = 0x0D;                          
	}
  	while(!(U0LSR & 0x20));
		U0THR = Ch;
	}
}
*/
	

