#include "lpc214x.h"
#include "SPI.h"
#include "card.h"
#include "interrupts.h"
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
#include "kernel.h"


void (*kernel_entry_point)();
unsigned char kernelMemory[3717];

void switchToUserMode(){
	unsigned int cpsrValue;
	
	__asm {
		MRS cpsrValue, CPSR
	}
	
	cpsrValue &= 0xFFFFFFF0;
	
	__asm {
		MSR CPSR_cxsf, cpsrValue
	}
}

int main(){		
	
	UartInit(9600);		 							//Initialize UART with 9600 baudrate
	printf("\nHello\n");
	printf("Program for MicroSD card\n\n");  
	
	card_init(); // Initialise card
	enable_interrupts();	
	
	
	load_kernel(0x00, kernelMemory);	
	
	
	
	//load_kernel_safe(kernelHexData, kernelMemory, kernelSize);
	//kernel_entry_point = (void (*)())kernelHexData;	
	kernel_entry_point = kernelHexData;	
	printf("Declared entry point");
	kernel_entry_point();
	printf("Called entry point");
	
	
	//switchToUserMode();		// Switch to user mode	
	
	
	// Halt
	while(1);
}
