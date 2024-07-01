#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_LEN 65 //Buffer of 64 characters and \n - Worst case

int main(void){
    
    //Thank you embedded ninja: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    //Initilize
    int fd;
    const char serial_port[] = "/dev/pts/3"; //Used by host in serial communication
    char buffer[BUF_LEN], result[BUF_LEN];
    struct termios termios_conf;
    
    //Open serial port device
    fd = open(serial_port, O_RDWR | O_NOCTTY );
    //Check for errors
    if (fd == -1){
        printf("Failed to open serial port\n");
        return 1;
    }
    
    // Read in existing settings, and handle any error
    tcgetattr(fd, &termios_conf);
    //Flag Initialization - Set Control Mode
    termios_conf.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    termios_conf.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    termios_conf.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    termios_conf.c_cflag |= CS8; // 8 bits per byte (most common)
    termios_conf.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    termios_conf.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    termios_conf.c_lflag &= ~ICANON; // Disable Canonical Mode
    termios_conf.c_lflag &= ~ECHO; // Disable echo
    termios_conf.c_lflag &= ~ECHOE; // Disable erasure
    termios_conf.c_lflag &= ~ECHONL; // Disable new-line echo
    termios_conf.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    termios_conf.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off software flow ctrl
    termios_conf.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    termios_conf.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    termios_conf.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        
    termios_conf.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    termios_conf.c_cc[VMIN] = 0;      
    //This is a blocking read of any number chars with a maximum timeout (given by VTIME).
    //read() will block until either any amount of data is available, or the timeout occurs.

    // Set in/out baud rate
    if(cfsetispeed(&termios_conf, B9600) < 0 || cfsetospeed(&termios_conf, B9600) < 0) {
        printf("Problem with baudrate\n");
        return 1;
    }
    //Saving termios
    if (tcsetattr(fd, TCSANOW, &termios_conf) != 0) {
        printf("Couldn't apply settings\n");
        return 1;
    }

    //Starting
    printf("Please give a string of up to 64chars to send to host:\n");
    fgets(buffer, BUF_LEN, stdin);

    tcflush(fd, TCIOFLUSH); /* Clear everything that is in the terminal*/

    //Sent to guest
    write(fd, buffer, BUF_LEN);

    //Blocking read from guest
    while (read(fd, result, 3) <= 0); 
    printf("The most frequent character is\n%c\nand it appeared %d times.\n", result[0], result[2]);

    //Done with serial port
    close(fd);
    return 0;
}
