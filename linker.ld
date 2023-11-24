MEMORY 
{
RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 0x1000
}

SECTIONS
{
.kernel_section :
{
	*(.kernel_section)
} > RAM
}