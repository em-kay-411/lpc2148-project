#include<lpc214x.h>
#include "SPI.h"
#include "card.h"
#include "interrupts.h"


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
	
	// Declaring a function pointer to the address where kernel is being loaded in the RAM
	void (*kernel_entry_point)() = (void (*)())KERNEL_LOAD_ADDRESS;
	
	//card_init(); // Initialise card
	enable_interrupts();	
	load_kernel();
	
	switchToUserMode();		// Switch to user mode
	kernel_entry_point();	
	
	
	// Halt
	while(1);
}
