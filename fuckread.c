#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/times.h>

int Uart_fd;
FILE *gps_fp;
unsigned char buffer[256];

int uart_setup(unsigned int baud_rate)
{
	int    ret = 0;
	struct termios termios_now;
	struct termios termios_new;
	struct termios termios_chk;

	if( Uart_fd > 0 )
	{
		memset( &termios_now, 0x0, sizeof(struct termios));
		memset( &termios_new, 0x0, sizeof(struct termios));
		memset( &termios_chk, 0x0, sizeof(struct termios));
	}

	tcflush(Uart_fd, TCIOFLUSH);
	tcgetattr(Uart_fd, &termios_new);
	termios_new.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	termios_new.c_oflag &= ~OPOST;
	termios_new.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	termios_new.c_cflag &= ~(CSIZE | PARENB);
	termios_new.c_cflag |= CS8;
	termios_new.c_cflag &= ~CRTSCTS;
	termios_new.c_cc[VTIME] = 0;  /* Over than VTIME*0.1 second(s), read operation will return from blocked */
	termios_new.c_cc[VMIN]  = 1; /* Over than VMIN bytes received, read operation will return from blocked */
	tcsetattr(Uart_fd, TCSANOW, &termios_new);
	tcflush(Uart_fd, TCIOFLUSH);
	tcsetattr(Uart_fd, TCSANOW, &termios_new);
	tcflush(Uart_fd, TCIOFLUSH);
	tcflush(Uart_fd, TCIOFLUSH);
	cfsetospeed(&termios_new, baud_rate);
	cfsetispeed(&termios_new, baud_rate);
	tcsetattr(Uart_fd, TCSANOW, &termios_new);

	if ( ret == 0 )
	{
		/* Write back finished, so read the settings again to check the content */
		ret = tcgetattr( Uart_fd, &termios_chk );
		if( ret == 0 )
		{
			if( termios_new.c_cflag != termios_chk.c_cflag )
			{
				printf("Failed to set the parameters of termios");
			} else
			{
				printf("Set the paremeters successfully\n");
				ret = 0;
			}
		}
	}
	return ret;
}

int main(int argc, char **argv)
{
    int ret = 0, count = 0, size;
    char buf[1];
    FILE *fp;

    printf("Build on %s\n", __TIMESTAMP__);
    if (argc != 2) {
        printf("Usage: %s [size]\n", argv[0]);
        exit(1);
    }

    Uart_fd = open("/dev/tty.usbserial-A600aeCk", O_RDWR | O_NOCTTY | O_NONBLOCK);
    uart_setup(B19200);

    size = atoi(argv[1]);

    fp = fopen("a.bin", "wb");

    do {
        ret = read(Uart_fd, &buf[0], 1);
	if (ret < 0)
		continue;
	if (buf[0] == '\n')
		count++;
	fprintf(fp, "%c", buf[0]);
	printf("Read %d record\r", count);
	fflush(NULL);
    } while (count < size);

    printf("Read %d record\n", count);

    fclose(fp);

    close(Uart_fd);
    return 0;
}
