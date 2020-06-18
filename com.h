#ifndef PORT_H
#define	PORT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/select.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
    
#include <stdio.h>        // ввод-вывод
#include <string.h>       // строковые функции
#include <unistd.h>       // стандартные функции UNIX
#include <fcntl.h>        // управление файлами
#include <termios.h>      // управление POSIX терминалом
#include <stdint.h>       // для типа переменных unit8_t
#include <errno.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <time.h>

#include <termios.h>
    
#define PAZM_RMC_SIZE 2*sizeof("$GPRMC,hhmmss.sss,A,GGMM.MM,P,gggmm.mm,J,v.v,b.b,ddmmyy,x.x,n,m*hh<CR><LF>")

    
int com_port_open(char *com, int speed_channel);
char *com_port_read(int fd, int *size, int time_wait);
void com_port_close(int fd);

#ifdef	__cplusplus
}
#endif

#endif	/* PORT_H */

