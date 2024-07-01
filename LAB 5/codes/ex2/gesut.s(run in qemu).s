.global main
.extern tcsetattr

main:
	ldr R0,=path	//Returns the file descriptor for the serial port
	MOV R1,#255	//The file descriptor fd returns in register R0
	ADD R1,R1,#3
	MOV R7,#5
	SWI 0

	MOV R10,R0 	//R10 will contain the file descriptor and will not change

	MOV R1,#0	//Sets the options for the termios
	LDR R2,=options
	BL tcsetattr

	MOV R0,R10	//We read from file descriptor up to 65 characters and store them in buffer. The last character will always be <Enter>
	LDR R1,=buffer
	MOV R2,#65
	MOV R7,#3
	SWI 0

	LDR R9,=counter	//R9 will have the address of the counter
	MOV R11,#0	//We will use R11 to have the index of the array
	LDR R12,=buffer	//We have the address of the buffer in R12

READING_LOOP:
	LDRB R2,[R12,R11] // R2 = Memory[R12+R11] = buffer[R11]
	ADD R11,R11,#1    // R11++ to take all the chars from buffer 
	CMP R2,#'\n'      //check if is the end of the string  if not make all the "NE" else continue
	SUBNE R2,R2,#32   // R2 = R2-32; take the index of the char 
	LDRNEB R3,[R9,R2] // R3 = counter[R2]
	ADDNE R3,R3,#1    // R3 ++ ; because we find again this char 
	STRNEB R3,[R9,R2] // counter[R2] = R3 , or acutally counter[R2]++
	BNE READING_LOOP  // go to next char of the string 	

	LDR R9,=counter //R9 will have the address of the counter
	MOV R11,#1	//We will use R11 as the index of array. We strat from 1 because the first is <SPACE> and do not count it
	MOV R0,#0 	//In R0 we will keep the frequency
	MOV R1,#0	//In R1 we will keep the index of the character character
	MOV R2,#0	//R2 will be the max frequency

MAX_LOOP:
	LDRB R0,[R9,R11] // R0 =  counter [R11]
	CMP R0,R2        // if Counter[R11] > MAXfreq 
	MOVHI R2,R0      // then MAXfreq = counter[R11]
	MOVHI R1,R11     // then index_of_ascii = R11 
	ADD R11,R11,#1   //next Counter element 
	CMP R11,#96      // stop when check all ascii characters 
	BLT MAX_LOOP

	ADD R1,R1,#32
	LDR R3,=output
	STRB R1,[R3]
	STRB R2,[R3,#1]
	MOV R1,#'\n'
	STRB R1,[R3,#2]

	MOV R0,R10
	LDR R1,=output
	MOV R2,#3
	MOV R7,#4
	SWI 0

End:
	MOV R7,#1
	SWI 0

.data    
 options: .word 0x00000000 /* c_iflag */
             .word 0x00000000 /* c_oflag */
             .word 0x000008b0 /* c_cflag */
             .word 0x00000002 /* c_lflag */
             .byte 0x00       /* c_line */
             .word 0x00000000 /* c_cc[0-3] */
             .word 0x00000000 /* c_cc[4-7] */
             .word 0x00000000 /* c_cc[8-11] */
             .word 0x00000000 /* c_cc[12-15] */
             .word 0x00000000 /* c_cc[16-19] */
             .word 0x00000000 /* c_cc[20-23] */
             .word 0x00000000 /* c_cc[24-27] */
             .word 0x00000000 /* c_cc[28-31] */
             .byte 0x00       /* padding */
             .hword 0x0000    /* padding */
             .word 0x0000000d /* c_ispeed */
             .word 0x0000000d /* c_ospeed */



path: .asciz "/dev/ttyAMA0"
buffer: .space 65
output: .space 3
counter: .ascii "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"	//We will have a counter for every possible printable ascii code