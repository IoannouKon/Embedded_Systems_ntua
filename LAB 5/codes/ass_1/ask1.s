.section .data
msg1: .asciz "Enter a string:"
msg2: .asciz "Input a string of up to 32 chars long:\n"
msg3: .asciz "Exit...\n"
msg4: .asciz "Ouput string is:"
format_str: .asciz "%s\n" // format string for scanf 

.section .bss
string_buffer: .space 100  // allocate space for the input  string buffer
output_buffer: .space 33 // allocate space for the ouput string buff

.section .text
.global _start
.global printMessage
.global scanf_string
.global Trasnfrom_string

_start:
 //printf(Enter string)
 MOV R0,#1          // file descriptor:1 (stduout)
 LDR R1, =msg1      // address of message
 LDR R2, =16        // length of the message
 BL printMessage    // Call the printMessage function for msg1 

 //scanf(string_buffer)
   //LDR R0,=format_str
   BL scanf_string


    //while loop 
     LDR R4, =output_buffer
     LDR R2, =string_buffer  // load the address of the buffer into R2
     LDRB R1,[R2,#1] // R0 =string[1] 
     CMP R1,#'\n'    // chech if R0 is the end of the string , check if the string have only one char    
     BEQ Check_Qq
   for_loop:
      LDRB R3,[R2] // Load the first byte of the string into r3 and increment r2 by 1
      CMP R3,#'\n'       // check if string is over
      BEQ NEXT_STRING
      B Trasnfrom_string
      continue:
      STRB R0, [R4]      // Store the byte in R0 to the memory location pointed to by R4
      ADD R2,R2,#1 
      CMP R2,#32       // check if string is over 32 bytes then stop
      BEQ NEXT_STRING
      ADD R4,R4,#1
      B for_loop
   NEXT_STRING:
        STRB R3, [R4]
        //printf(Output string)
          MOV R0,#1          // file descriptor:1 (stduout)
          LDR R1, =msg4      // address of message 
          LDR R2, =16        // length of the message 
          BL printMessage    // Call the printMessage function for msg4
          BL _printBuffer
          B _start

  EXIT:
    MOV R0,#1          // file descriptor:1 (stduout)
    LDR R1, =msg3      // address of message 
    LDR R2, =10         // length of the message 
    BL printMessage    // Call the printMessage function for msg1
   mov r7,#1 // syscall number for exit
   swi 0


Trasnfrom_string:
//R3 is input (char)  and R0 is ouput (char) 
tolower:
     CMP  R3, #'A'              // Compare with ASCII code for 'A'
     BLT  not_uppercase        // Branch if the character is below 'A'
     CMP  R3, #'Z'             // Compare with ASCII code for 'Z'
     BGT  not_uppercase         // Branch if the character is above 'Z'
     SUB R3,R3,#'A' //caclulate the offset
     ADD R3,R3,#'a' //add the offset to convert to lowercase
     B RETURN
not_uppercase:
     CMP  R3, #'a'              // Compare with ASCII code for 'A'
     BLT  not_lowercase        // Branch if the character is below 'A'
     CMP  R3, #'z'             // Compare with ASCII code for 'Z'
     BGT  not_lowercase         // Branch if the character is above 'Z'
     SUB R3,R3,#'a'           //caclulate the offset
     ADD R3,R3,#'A'            //add the offset to convert to lowercase
     B RETURN
not_lowercase:
     CMP R3, #'0' 
     BLT not_in_ragne1
     CMP R3,#'4'
     BGT not_in_ragne1
     ADD R3,R3,#4
     ADD R3,R3,#1
     B RETURN
not_in_ragne1:
     CMP R3, #'5' 
     BLT RETURN
     CMP R3,#'9'
     BGT RETURN
     SUB R3,R3,#5
 RETURN:
    MOV R0,R3
B continue


Check_Qq:
   LDRB R1,[R2,#0] // R0 =string[0] 
   CMP  R1,#'Q'
   BEQ  EXIT
   CMP  R1,#'q'
   BEQ  EXIT
   MOV R1,#0 
   B   for_loop      //jump to for_loop

printMessage:
    // Function to print a message
    // Input: R0 - file descriptor, R1 - address of the message, R2 - length of the message
    MOV R7, #4          // syscall number for write
    SWI #0              // Make a syscall
    BX LR               // Return from the function

scanf_string:
    MOV R0,#0                    // File descriptor: STDIN
    LDR R1,=string_buffer         // Buffer address
    MOV R2,#100                   // Maximum number of characters to read
    MOV R7,#3                    // System call number for read
    SWI 0                        // Make a syscall   
    BX LR                        // Return from the function


_printBuffer:
    MOV R0, #1               // File descriptor 1 (STDOUT)
    LDR R1, =output_buffer    // Address of the output buffer
         LDRB R2, ='\n'            // Load the newline character into R2
         STRB R2, [R1, #31]        // Store '\n' at the 32nd position in output_buffer
    LDR R2, =32             // Number of bytes to write (adjust based on your buffer size)
    MOV R7, #4               // syscall number for write
    SWI #0                   // Make a syscall
    BX LR

