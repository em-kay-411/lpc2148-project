#include "lpc214x.h"
#include "interrupts.h"

void softwareInterruptHandler(void) __irq
{
    // Your code for the software interrupt handler goes here

    // Clear the interrupt in the VIC
    VICVectAddr = 0;
}

void enable_interrupts(void){
	VICVectAddr = 0;  // Clear any pending interrupts
  VICIntSelect &= ~0x40000000;  // Set the software interrupt as IRQ
  VICIntEnable = 0x40000000;  // Enable the software interrupt

    // Set the vector address for the software interrupt
  VICVectCntl1 = 0x20 | 0x1;  // Enable vector interrupt slot 1 and assign it to the software interrupt
  VICVectAddr1 = (unsigned int)softwareInterruptHandler;  // Set the address of the handler function

    // Enable the interrupt in the ARM core
  __enable_irq();
}
