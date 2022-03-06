# Intel 4004 Emulator

Simple 4004 emulator written in C.

to use, open a raw binary file (not intel hex ^yet ) in main.c and create a 4001 rom device with it,
then attach that to the cpu with rom id 0.

each time execute_cpu is called, the cpu will perform one instruction and print out the resulting status
afterwards.


if you want to add more devices, it needs to implement the read, write, and destroy functions as defined
in the memory_node struct in 4004.h. You also need to be sure to add them as a ram device or a rom device
since those are addressed seperately. 

NOTE: ram status characters don't work right now, you can only read the regular data. I want to keep it all
generic but ram and rom devices might have to be different since the status bytes are addressed differently.
