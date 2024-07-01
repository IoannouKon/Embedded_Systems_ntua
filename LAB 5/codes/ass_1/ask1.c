#include <stdio.h>
#include <ctype.h>

char UP(char ch) {
    if(isupper(ch)) 
            { 
            ch = tolower(ch);
            }
    if(islower(ch)) 
            { 
            ch = toupper(ch);
            }       
     return ch;       
}

char code_digits(char ch) { 
    if (ch >= '0' && ch<= '4') 
            { 
                ch = ch +5 ;
            }
            else if (ch >= '5' && ch<= '9') 
            {
               ch =ch -5;
            }
                 return ch;       
}


int main() {
    char userInput[32]; 
    char temp;
    size_t length ; 
    
    printf("Enter a string: ");
    scanf("%31s", userInput); 
    printf("You entered: %s\n", userInput);
    length = strlen(userInput);
    
     while( (userInput[0] != 'q' && userInput[0] != 'Q') || length >1 ) 
    {
        for(int i =0; i<31;i++) {
              userInput[i] =UP(userInput[i]);
              userInput[i]=code_digits(userInput[i]);
        }
             printf("Output string is: %s\n", userInput);
         
            printf("Enter a  string: ");
            scanf("%31s", userInput); 
            printf("You entered: %s\n", userInput);
            length = strlen(userInput);
    
    }    
 printf("Exit....: ");
    return 0;
}

