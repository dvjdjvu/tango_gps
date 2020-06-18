#include "com.h"

int
com_port_open(char *com, int speed_channel) {
    int fd;

    //fd = open(com, O_RDWR | O_NOCTTY | O_SYNC);
    fd = open(com, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        return fd;
    }

    struct termios options; // настройка порта
    tcgetattr(fd, &options);
    cfsetispeed(&options, speed_channel);
    cfsetospeed(&options, speed_channel);

    options.c_line = 0;

    options.c_iflag |= IGNBRK; // ignore BREAK condition on input 
    options.c_iflag |= IGNPAR; // ignore framing errors and parity errors 
    options.c_iflag &= ~PARMRK;
    options.c_iflag &= ~INPCK; // disable input parity checking 
    options.c_iflag &= ~ISTRIP; // disable strip off eighth bit 
    options.c_iflag &= ~INLCR; // don't map NL to CR-NL on input 
    options.c_iflag &= ~IGNCR; // don't ignore carriage return(CR) on input 
    options.c_iflag &= ~ICRNL; // don't map CR to NL on input 
    options.c_iflag &= ~IUCLC; // don't map uppercase->lowcase on input 
    options.c_iflag &= ~IXON; // disable XON/XOFF flow control on input  
    options.c_iflag |= IXANY;
    options.c_iflag &= ~IXOFF; // disable XON/XOFF flow control on output 
    options.c_iflag &= ~IMAXBEL; // not implemented in Linux 

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    options.c_cflag |= (CLOCAL | CREAD); // Enable the receiver and set local mode
    options.c_cflag &= ~PARENB; // No parity bit
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE; // Mask data size
    options.c_cflag |= CS8; // Select 8 data bits
    options.c_cflag |= CRTSCTS;
    //options.c_cflag &= ~CRTSCTS;            // Disable hardware flow control 
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;

    tcflush(fd, TCOFLUSH | TCIFLUSH);

    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

char *
com_port_read(int fd, int *size, int time_wait) {
    int ret, i = 0;
    fd_set fds;

    char ch;
    char *str;

    // Тайм-аут приема данных.
    struct timeval tv;
    tv.tv_sec = time_wait;
    tv.tv_usec = 0;


    if (time_wait > 0) {
        // Очищаем весь набор.
        FD_ZERO(&fds);

        // wait for read
        FD_SET(fd, &fds);
        ret = select(fd + 1, &fds, NULL, NULL, &tv);
        if (ret <= 0) {
            return NULL;
        }
    }
    //ioctl(fd, FIONREAD, &size);

    *size = 0;
    str = (char *) calloc(PAZM_RMC_SIZE, sizeof (char));
    if (str == NULL) {
        return NULL;
    }

    for (;;) {
        read(fd, &ch, sizeof (char));
        if (ch == '$') {
            str[i++] = ch;
            break;
        }
    }

    while (ch != '\n') {
        *size += read(fd, &ch, sizeof (char));
        str[i++] = ch;
        //printf("%c", ch);
    }

    str[i++] = '\0';
    
    return str;
}

void
com_port_close(int fd) {
    close(fd);
}