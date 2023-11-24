#include "lpc214x.h"
#include "SPI.h"
#include "card.h"
#include "interrupts.h"
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
#include "kernel.h"


void (*kernel_entry_point)();
unsigned char kernelMemory[228];


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
	kernel_entry_point = (void (*)())kernelMemory;	
	//kernel_entry_point = kernelHexData;	
	printf("Welcome to the Kernel! Type from your keyboard to display.\n\n");
	kernel_entry_point();
	
	
	// Halt
	while(1);
}
