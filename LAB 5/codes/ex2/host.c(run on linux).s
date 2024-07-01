#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include"termios.h"

int main()
{

    int fd;            //File descriptor used for communication
    char input=NULL;        //Pointer to the user's string in terminal
    char output[3];        //The most frequent character and the frequency in this array
    ssize_t counter;
    struct termios tty;
    charc;
    size_t length = 65;
 
    printf("Input of up to 64 bit to send:\n");
    counter = getline(&input, &length, stdin);

    if (counter<0){
        perror("read");
          exit(1);
    }

    length = counter;

    if (length >65) {
          fprintf(stderr, "The input is too large\n");
           fflush(0);
          exit(1);
    }

    else
        fd = open("/dev/pts/2", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        printf("Failed to open port\n");
        return 1;
    }

    if(tcgetattr(fd, &tty) < 0) {
        printf("Couldn't get the data from the terminal\n");
        return 1;
    }



    tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    tty.c_lflag=ICANON;
    tty.c_cflag=CS8|CREAD|CLOCAL;

    if(cfsetispeed(&tty, B9600) < 0 || cfsetospeed(&tty, B9600) < 0) {
        printf("Problem with baudrate\n");
        return 1;
    }

    if(tcsetattr(fd, TCSANOW, &tty) < 0) {
        printf("Couldn't apply settings\n");
         return 1;
    }

    tcflush(fd, TCIOFLUSH); 
    write(fd, input, counter);
    read(fd, output,3);
    int frequency;
    frequency = output[1]-'0';
    printf("The most frequent character is "%c" \nand it appeared %d times.\n", output[0], frequency);
    close(fd);
    return 0;

}