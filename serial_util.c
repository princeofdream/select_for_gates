#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


#undef EVEN_PARITY
#define EVEN_PARITY

#define BAUDRATE B115200
/* #define BAUDRATE B38400 */



static void serial_setup_tio(int fd)
{
	struct termios newtio;

	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

#ifdef EVEN_PARITY
	//Even parity
	newtio.c_cflag &= ~PARENB; /*  Clear parity enable */
	newtio.c_iflag &= ~INPCK; /*  Enable parity checking */
	newtio.c_iflag |= INPCK; /*  Disnable parity checking */

	newtio.c_cflag |= PARENB; /*  Enable parity */
	newtio.c_cflag &= ~PARODD; /*  偶效验 */
	/* newtio.c_iflag |= (INPCK|ISTRIP); */
	/* newtio.c_cflag |= PARENB;         */
	/* newtio.c_cflag &= ~PARODD;        */
#endif

	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
}

int serial_open(const char *fn, struct termios *tio_saved)
{
	int fd;

	fd = open(fn, O_RDWR | O_NOCTTY);
	if (fd < 0)
		return fd;

	tcgetattr(fd, tio_saved);
	serial_setup_tio(fd);
	return fd;
}

void serial_close(int fd, const struct termios *tio_saved)
{
	tcsetattr(fd, TCSANOW, tio_saved);
	close(fd);
}
