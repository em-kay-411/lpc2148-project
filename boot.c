#include<lpc214x.h>

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
	switchToUserMode();
	
	while(1);
}
