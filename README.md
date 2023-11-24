# Basic Boot Loader Implementation on ARM LPC2148
### ABSTRACT
The project presents the design and implementation of a basic boot loader for an ARM core LPC2148 microcontroller board. The boot loader plays a pivotal role in the initialization of the system, including the configuration of essential peripherals, storage devices, and interrupt handling. The primary objective is to seamlessly facilitate the loading of the kernel into RAM from a MicroSD card, thus enabling a smooth transition to the kernel execution phase.
The boot loader interacts with the MicroSD card, which serves as the repository for the kernel image. The boot loader orchestrates the retrieval and loading of the kernel into the system's RAM, laying the groundwork for the seamless transition to kernel execution.


### KERNEL
The kernel could also be a standalone application running on top of the boot loader. The kernel in this project, being a basic implementation, is approximately a 228 KB image stored in the SD Card. Its function is to take any input from the user keyboard, send via UART to the microcontroller, and echo the same back to the user on the terminal. 
The hex values from the SD Card are taken up block by block and then loaded on to the RAM. We need to make sure that the address that it is being loaded has permissions to execute. 
The kernel is compiled separately and converted to a disk image which is then inserted into the SD Card. The kernel basically works on top of the Boot Loader and everything that has been initialised. So we need to have the one compiled according to the initialisations of our boot loader. 
To do so, the kernel hex values are carefully identified from the memory window in Keil and manually written to every location in the SD Card. Thus, the functions that the kernel is running are on top of the initialisations made by the boot loader.


### SD CARD
The SD Card, unlike normal SD Cards, contains raw binary executable data of the kernel. It does not have a filesystem associated with it. The kernel image is created by compiling the kernel and converting the hex file to a disk image file. The boot sector of the card consists of partition tables according to the sector size. But due to raw kernel being loaded, the sectors cannot be accessed directly.  
The kernel size is 228 bytes and the kernel.img file is written to the Sector 0 of the SD Card. Being lesser than 512 bytes, the kernel fits in the Sector 0 itself, which makes it easy to access thus not requiring any memory mapping of logical addresses.
The microcontroller interacts with the SD Card using SPI by sending commands and getting acknowledgements for those. These commands are of 6 bytes each of them consisting of the hex values corresponding to the function and the required parameters in the later bits. 
The load_kernel() function in card.c is responsible for loading the kernel into the memory. It takes in the sector of the card to be read and the memory array where the kernel image has to be stored. While loading, every byte that is read is displayed on to the terminal until the entire kernel loads. 

### RESULTS
•	The SD Card is initialised successfully, indicated by the SPI response.
•	All 228 bytes of the kernel load successfully into the RAM.
•	The function to echo every keyboard interrupt on the host system, which is defined here as the kernel, takes control and starts execution.

