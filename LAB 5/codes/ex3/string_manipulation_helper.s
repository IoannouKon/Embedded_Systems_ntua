
.text
.align 4
//.global strlen
.type strlen %function
strlen:
  MOV R1, R0      // Copy the address of the input string to R1
  MOV R0, #0      // Initialize the length counter to 0

 strlen_loop:
  LDRB R2, [R1], #1  // Load the byte at the address in R1 and increment the address
   CMP R2, #0        // Compare the loaded byte with null terminator
   BEQ strlen_end
   ADD R0,R0,#1
   B strlen_loop

 strlen_end:
  BX LR     // return with the length in R0 regiser

.align 4
.global strcpy
.type strcpy  %function
strcpy:
  //RO = output_file , R1 = input_file
  strcpy_loop:
   LDRB R2, [R1], #1  // Load the byte at the address in R1 and icrement the address
   STRB R2,[R0],#1    // store the value in the register R0 (pointer
   CMP R2, #0        // Compare the loaded byte with null terminator
   BNE  strcpy_loop
   BX LR     // return with register R0,R1

.align 4
.global strcat
.type strcat  %function
strcat:
  // input: RO = destination(string1) , R1 = source(string2)
  // output:RO = string1+string2 , R1=string2

 // find the length of str1 -- go to the end of that string
 len_loop:
 LDRB R2,[R0],#1
 CMP R2,#0
 BNE len_loop

 SUB R0,R0,#1 // go string1 before \n at the last charachter of string1

 copy_loop:
 LDRB R2,[R1],#1
 STRB R2,[R0],#1
 CMP R2,#0
 BNE copy_loop

 BX LR
 
 .align 4
.global strcmp
.type strcmp  %function
strcmp:
   // input R0 = str1  R1 =str2
   // return 0 if the string are equal (the reutrn value is stored at register R0)
   // >0 if str1 >str2   else < 0
   compare_loop:
   CMP R2,#'\n' // check if both of the string has finished
   BEQ Finish
   LDRB R2,[R0],#1 // R2 = str1[i] i++
   LDRB R3,[R1],#1 // R3 =str2[j] j++
   CMP R2,R3
   BEQ compare_loop

   CMP R2,R3
   BGT  RETURN_1       //if str1[i] > str2[i]
   MOV R0,#-1
   BX LR
   RETURN_1:
   MOV R0,#1
   BX LR

Finish:
 MOV R0,#0
 BX LR



