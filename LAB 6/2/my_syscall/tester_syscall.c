#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h> 

#define my_sys_call 386 

int main(void) {
    long result; 
    printf("Here i call my dummy sys call...\n");
    result = syscall(my_sys_call);
    printf("the system call return: %d. \n",result);
    return 0;
}


