#ifndef KERNEL_H
#define KERNEL_H

//unsigned int kernelSize = 3717;

//unsigned char kernelMemory[3717];

//unsigned char kernelHexData[] = {0x20, 0x00, 0x8F, 0xE2, 0x9C, 0xFE, 0xFF, 0xEB, 0x3C, 0x00, 0x8F, 0xE2, 0x9A, 0xFE, 0xFF, 0xEB, 0x03, 0x00, 0x00, 0xEA, 0x9D, 0xFF, 0xFF, 0xEB, 0xFF, 0x40, 0x00, 0xE2, 0x04, 0x00, 0xA0, 0xE1, 0xA4, 0xFF, 0xFF, 0xEB, 0xFA, 0xFF, 0xFF, 0xEA};

void kernelHexData(void);

void load_kernel_safe(unsigned char[], unsigned char[], unsigned int);
#endif