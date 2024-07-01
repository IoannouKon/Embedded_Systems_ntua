.section .data

output_msg1: .asciz "Input a string up to 32 characters long:"

length1=.-output_msg1

output_msg2: .asciz "Result:"

length2=.-output_msg2

output_msg3: .asciz "Exiting...\n"

length3=.-output_msg3

character:.space 1 //We use this as a variable to read every character

buffer:.space 33 //We want the buffer to keep the first 32 characters and the last one is <ENTER>



.section .text

.global main



main:

	LDR R1,=output_msg1	//We start the programm and print the msg1

	LDR R2,=length1

	BL printf

	

	//We read the string with a while-loop and store it in the buffer

	MOV R11,#0 //We use R11 as a counter 

	LDR R12,=buffer

READING_LOOP:		

	BL scanf		//We read one letter in the character array

	LDR R0,=character	//We get the address of character in R0

	LDRB R1,[R0]		//We load the character we read in R1

	CMP R1,#'\n'		//If it is <Enter> the flag Z is set and dont check if we have more than 32 characters

	CMPNE R11,#32		//If flag Z is 0 then we must check if we have more than 32 characters

       	ADDNE R11,R11,#1        //We increase the counter

	STRNEB R1,[R12],#1 	//If either comparison do not set the flag Z then we put it in the buffer and then increase counter 

	BNE READING_LOOP	//Here the reading is over

	

	MOV R2,#'\n'		//We add one last <Enter> in the buffer

	STRB R2,[R12],#1	//R12 shows one address more than buffer's end 

	ADD R11,R11,#1		//We count the <Enter> we add too

				

	CMP R1,#'\n'		//If last character is not <Enter> flash the cache

	BLNE flush_cache	//The cache is flashed by reading all the characters left and when it reads \n it stops



	LDR R0,=buffer		//We check is the user gave us 'Q' or 'q'	

	LDRB R1,[R0]		//We get the first character of buffer

	CMP R1,#'q'		//If it is q we dont check if it is Q and we check the number of total charcters

	CMPNE R1,#'Q'		//If it is not q we check if it is Q. If so then we check the number of characters else the flag Z is 0 and we skip the next 2 intructions

	CMPEQ R11,#2		//We check the number of charcters with 2 beacuse we have also included <Enter>

	BEQ END			//If (character==2 &&(buffer[0]=='q' || buffer[0]=='Q')) the programm ends

	

	LDR R0,=buffer 		//R0 show in the beggining of the buffer and R12 shows in the end

	BL manipulation



	LDR R1,=output_msg2	//We print the result

	LDR R2,=length2

	BL printf

	LDR R1,=buffer

	MOV R2,R11

	BL printf

	B main	

	

END:	

	LDR R1,=output_msg3

	LDR R2,=length3

	BL printf

	MOV R7,#1     		//The syscall with R7=1 ends the program

	SWI 0                 



printf:

	MOV R0,#1 		//We want to print the message in the stdout

	MOV R7,#4 		//The syscall with R7=1 uses write()

	SWI 0	 		//We do the system call for printing out

	BX lr			//We return to the main program

scanf:

	MOV R0,#0

	LDR R1,=character

	MOV R2, #1      

        MOV R7, #3       

        SWI 0 

	BX lr

flush_cache:

	PUSH {lr}		//We save lr beacuse we call scanf and lose the lr to return into main program

	BL scanf		//We read charactres until we rea <Enter>

	LDR R0,=character

	LDRB R1,[R0]

	CMP R1,#'\n'

	BNE flush_cache

	POP {lr}

	BX lr

manipulation:

	LDRB R1,[R0]

	CMP R1,#'z'		//Compare it with 'z'

	BHI return		//If R1>'z' then we do not make changes

	CMP R1,#'a'		//We compare it with 'a' which means R1<'z'

	SUBGE R1,R1,#32 	//If R1>='a' it means 'a'<=R1<='z' and we substract 32 beavuse Capitals are 32 positions lower in Ascii table

	BGE return		//If it is >='a' then we go to the end or else we continue



        CMP R1,#'Z'     	//In this line we know R1 is not a lower case letter so we compare it with 'Z'

        BHI return		//If it is greater than Z and it is not a lower case it means we dont check it

        CMP R1,#'A'     	//Compare with 'A' which means R1<'Z'

        ADDGE R1,R1,#32 	//This command is excecuted if R1<='Z' and R1>='A' so R1 is capital and add 32 to make it lower

        BGE return		//If we found it as capital letter go to the return

	

        CMP R1,#'9'     	//Compare R1 with '9'. If the code is here it means it is not either lower case or capital

        BHI return		//If R1>'9' and taking into consideration is it not a letter we are done

        CMP R1,#'5'     	//Compare it with '5'

        SUBGE R1,R1,#5 		//If R1>='5' it means we have '5'<=R1<='9' and we sub 4 and we are done

        BGE return

				

        CMP R1,#'0'     	//If we get here R1<'5' so we check it is >=0

        ADDGE R1,R1,#5		//If so we add 5

return:	

	STRB R1,[R0],#1

	CMP R0,R12

	BNE manipulation

	BX lr