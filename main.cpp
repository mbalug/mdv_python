#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, char* argv[]) {

    struct termios serial;
    char* str = "Hello\n";
    char buffer[10];

    int socket = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

    if (socket == -1) {
        perror("/dev/ttyS0");
        return -1;
    }

    if (tcgetattr(socket, &serial) < 0) {
        perror("Getting configuration");
        return -1;
    }

    cfsetispeed(&serial,B9600); // set rx to 9600
    cfsetospeed(&serial,B9600); // set tx to 9600


    // Flush serial configuration
    serial.c_iflag = 0;
    serial.c_oflag = 0;
    serial.c_lflag = 0;
    serial.c_cflag = 0;

    // tcflush(socket, TCIFLUSH); -> discard old rx buffer data
    // serial.c_cc[VMIN] = 0;
    // serial.c_cc[VTIME] = 0;

    serial.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    serial.c_cflag = CS8 | CREAD | CLOCAL;

    // setup tx config
    serial.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
                     ONOCR | OFILL | OLCUC | OPOST | CRTSCTS);

    // setup rx config
    serial.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    serial.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* No Cannonical mode                            */


    tcsetattr(socket, TCSANOW, &serial); // Apply configuration

    // Attempt to send and receive
    printf("Sending: %s\n", str);

    int wcount = write(socket, str, strlen(str));
    if (wcount < 0) {
        perror("Write");
        return -1;
    }
    else {
        printf("Sent %d characters\n", wcount);
    }

    sleep(1);
    int rcount = read(socket, buffer, sizeof(buffer));
    printf("Received %d\n", rcount);
    if (rcount < 0) {
        perror("Read");
        return -1;
    }
    else {
        printf("Received %d characters\n", rcount);
    }

    buffer[rcount] = '\0';

    printf("Received: %s\n", buffer);

    close(socket);
}