#include "lpc214x.h"
#include "stdio.h"
#include "interrupts.h"
#include "uart.h"

void softwareInterruptHandler(void) __irq{
    VICVectAddr = 0;
    printf("Kernel enable to load. This is a custom interrupt");    
} 

void enable_interrupts(void){
	VICVectAddr = 0;  // Clear any pending interrupts
  VICIntSelect &= ~0x40000000;  // Set the software interrupt as IRQ
  VICIntEnable = (1 << 6);  // Enable the software interrupt

    // Set the vector address for the software interrupt
  VICVectCntl1 = 0x20 | 6;  // Enable vector interrupt slot 1 and assign it to the software interrupt
  VICVectAddr1 = (unsigned long)softwareInterruptHandler;  // Set the address of the handler function

    // Enable the interrupt in the ARM core
  __enable_irq();
}
