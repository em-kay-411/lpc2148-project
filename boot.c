#include<lpc214x.h>
#include "SPI.h"
#include "card.h"

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
	card_init();			// Initialise card
	switchToUserMode();		// Switch to user mode
	
	while(1);
}
