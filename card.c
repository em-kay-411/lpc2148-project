#include "lpc214x.h"
#include "stdio.h"
#include "math.h"
#include "SPI.h"
#include "card.h"
#include "UART.h" 
#define CD 1<<22		//Card Detect Pin


unsigned char CMD0[6] = {0x40,0x00,0x00,0x00,0x00,0x95};	//reset card into idle mode (response 0x01 if all OK)
unsigned char CMD1[6] = {0x41,0x00,0x00,0x00,0x00,0xFF};	//start initialization	(MMC and std SD cards only)(response 0x00 if all OK)
unsigned char CMD8[6] = {0x48,0x00,0x00,0x01,0xAA,0x87};	//check if standard SD or SDHC (V1 or V2 and later) (0x01->check voltage range 2.7-3.6V) (0xAA->Check Pattern) (expand ACMD41 capability for SDHC)
unsigned char CMD9[6] = {0x49,0x00,0x00,0x00,0x00,0x00};	//to read capacity of card
unsigned char CMD10[6]= {0x4A,0x00,0x00,0x00,0x00,0x00};	//to read name, serial no. etc
unsigned char CMD17[6]= {0x51,0x00,0x00,0x00,0x00,0x00};	//to read from a single sector (512 bytes)
unsigned char CMD24[6]= {0x58,0x00,0x00,0x00,0x00,0x00};	//to write to a single sector  (512 bytes)
unsigned char CMD41[6]= {0x69,0x40,0x00,0x00,0x00,0xFF};	//ACMD41 loop	(ACMD41 = CMD55 + CMD41) (synchronization command used to poll the card until it is out of its power-up sequence) (Response 0x00 when ready)
unsigned char CMD55[6]= {0x77,0x00,0x00,0x00,0x00,0xFF};	//Leading command for all ACMDs
unsigned char ch;											//for counting and temp purpose
unsigned int i;


unsigned char send_cmd(unsigned char *temp)
{
	spi0_read();											//dummy read; provide clock pulses to card to process previous work if any
	for(i=0;i<6;i++)									   	//send the 6 byte command
		spi0_write(temp[i]);
	ch = spi0_read();									  	//dummy read; provide clock pulses to card to process command
	return ch;
}

void card_init()
{
	unsigned int timer=100;

	PINSEL1 &= 0xFFFFCFFF; 									//configure P1.22 as GPIO for detecting card
	IODIR0 &= ~CD;										   	//CD pin as input

	if(IOPIN0 & CD)										   	//if card not inserted; wait till card is inserted
	{
		printf("Insert card");
		while(IOPIN0 & CD);
	}

	delay(5000);							   			//on power on, wait atleast 1m sec
	spi0_init();										//Initialize SPI

	printf("\nInitializing Card\n");
	for(i=0;i<10;i++)   								//send 80 clock pulses
		spi0_read();

	/* CMD0 routine*/
	printf("--CMD0--\n");
	IOCLR0 = SSEL0;								//chip select low
	send_cmd(CMD0);								 //send CMD0 to reset the card into SPI mode - IDLE state
	ch = spi0_read();							//if response 0x01... card is initialised for SPI protocol
	IOSET0 = SSEL0;								//chip select high
	delay(1000);
	if(ch == 0x01)
		printf("response = %02x\nCard configured in SPI mode\n",ch);
	delay(1000);

	/* CMD8 routine*/
	printf("--CMD8 with arguments 0x00 0x00 0x01 0xAA--\n");
	IOCLR0 = SSEL0;							//chip select low
	send_cmd(CMD8);							//send CMD8 to check card version, host voltage compatibility, 
	delay(1000);							//and expand ACMD41 capability if card is SDHC
	printf("response = 0x%02x\n",spi0_read());		  //should be 0x01, since CMD8 works in IDLE mode only
	printf("response arg =  ");						//should return arguments 0x00 0x00 0x01 0xAA if everything OK
	for(i=0;i<4;i++)
	{
		ch = spi0_read();
		printf("0x%02X ",ch);
	}
	IOSET0 = SSEL0;						 	//chip select high
	delay(1000);


	/* ACMD41 routine */
	printf("\n--ACMD41--\n");
	delay(1000);
	do
	{
		IOCLR0 = SSEL0;				   	//chip select low
		send_cmd(CMD55);			  	//send leading command for ACMD
		spi0_read();					//dummy read
		spi0_read();
		IOSET0 = SSEL0;					//chip select high

		delay(50);

		IOCLR0 = SSEL0;					//chip select low
		send_cmd(CMD41);				//send command 41 to complete the SD card power-up and initialization sequence
		ch = spi0_read();				//response 0x00 when ready
		IOSET0 = SSEL0;
		delay(50);
	}
	while(ch != 0x00 && --timer);
	if(timer == 0)
	 	printf("Card Initialization FAILED\n\n");
	else
		printf("Card Initialization SUCCESS\n\n");
}

void load_kernel(unsigned int sector)
{
	unsigned char temp[6] = {0,0,0,0,0,0};
	unsigned int timer = 300;
	unsigned int j = 0;
	
	
	printf("\n\nReading from sector 0x%x\n",sector);	
	printf("--CMD17--\n");

	sector = sector*512;						   	//convert sector address to byte address
	temp[0] = CMD17[0];
	temp[4] = sector & 0xFF;					   	//pass the address as arguments for CMD17
	temp[3] = (sector>>8) & 0xFF;
	temp[2] = (sector>>16) & 0xFF;


	IOCLR0 = SSEL0;								 	//chip select low
	send_cmd(temp);									//send CMD17 with arguments
	do												//wait till 0xFE is received indicating start of reading data
	{
		ch=spi0_read();							   	
	}
	while(ch != 0xFE && --timer);

	if(timer == 0)								  	//if timer runs out before 0xFE is received, Read Failed
	{
		printf("Read FAILED\n");
		while(1);
	}
	printf("Start reading\n");
	delay(1000);
	for(i=0;i<512;i++){				//read data
		if(ch == 0xFF || ch == 0x00){
			return;
		}
		ch=spi0_read();
		
		if(j == 16){
			printf("\n");
			j = 0;
		}
		printf("0x%02x ",ch);
		*((volatile unsigned char *)(KERNEL_LOAD_ADDRESS + i)) = ch;
		j++;
	}

	spi0_read();							   		//Read CRC (can be ignored)
	spi0_read();

	IOSET0 = SSEL0;								  	//chip select high
	printf("Read data from sector 0x%x = 0x%02x\n",sector,ch);
	printf("Read SUCCESS\n");
}

/*
void load_kernel(void)
{
    unsigned char temp[6] = {0, 0, 0, 0, 0, 0};
    unsigned int timer = 300;
    unsigned int kernel_size = 4096;// Calculate the size of your kernel image 

    printf("\n\nLoading Kernel from SD card\n");
    printf("--CMD17--\n");

    temp[0] = CMD17[0];
    temp[4] = 0; // Set the sector to 0 for the first sector
    temp[3] = 0;
    temp[2] = 0;

    IOCLR0 = SSEL0; // Chip select low
    send_cmd(temp); // Send CMD17 with arguments
    do
    {
        ch = spi0_read();
    } while (ch != 0xFE && --timer);

    if (timer == 0)
    {
        printf("Read FAILED\n");
        while (1);
    }

    printf("Start reading\n");
    delay(1000);

    // Read the entire kernel image
    for(i = 0; i < kernel_size; i++)
    {
        ch = spi0_read();
			
				printf("%c\n", ch);

        // Load the kernel code into RAM
        *((volatile unsigned char *)(KERNEL_LOAD_ADDRESS + i)) = ch;
    }

    spi0_read(); // Read CRC (can be ignored)
    spi0_read();

    IOSET0 = SSEL0; // Chip select high

    printf("Kernel Loaded SUCCESSFULLY\n");
}
*/
