#include "kernel.h"
#include "uart.h"
#include "stdio.h"

unsigned int s = 0;
void load_kernel_safe(unsigned char kernelHexData[], unsigned char kernelMemory[], unsigned int size){
for(s=0; s<size; s++){
	kernelMemory[s] = kernelHexData[s];
}
}

void kernelHexData(void){
   	printf("\r\nKernel loaded successfully....");
   	printf("\r\nHit any Key on your PC Keyboard to see input echoing\r\n\n\n");

	while(1)
	{
		UART_PutChar(UART_GetChar()); 		//Echo
	}
}
