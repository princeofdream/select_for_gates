#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


#undef EVEN_PARITY
#define EVEN_PARITY

#define BAUDRATE B115200
/* #define BAUDRATE B38400 */

static tcflag_t serial_cflag = 0;
static unsigned long s_baudrate;

static void serial_setup_tio(int fd)
{
	struct termios newtio;

	if (serial_cflag == 0) {
		serial_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	}
	newtio.c_cflag = serial_cflag;

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


void serial_set_cflags(tcflag_t flag)
{
	serial_cflag = flag;
	return;
}

tcflag_t serial_get_cflags()
{
	return serial_cflag;
}

void serial_set_baudrate(int rate)
{
	switch (rate) {
		case 50:
			s_baudrate = B50;
			break;
		case 75:
			s_baudrate = B75;
			break;
		case 110:
			s_baudrate = B110;
			break;
		case 134:
		s_baudrate = B134;
		break;
		case 150:
			s_baudrate = B150;
			break;
		case 200:
			s_baudrate = B200;
			break;
		case 300:
			s_baudrate = B300;
			break;
		case 600:
			s_baudrate = B600;
			break;
		case 1200:
			s_baudrate = B1200;
			break;
		case 1800:
			s_baudrate = B1800;
			break;
		case 2400:
			s_baudrate = B2400;
			break;
		case 4800:
			s_baudrate = B4800;
			break;
		case 9600:
			s_baudrate = B9600;
			break;
		case 19200:
			s_baudrate = B19200;
			break;
		case 38400:
			s_baudrate = B38400;
			break;
		case 57600:
			s_baudrate = B57600;
			break;
		case 115200:
			s_baudrate = B115200;
			break;
		case 230400:
			s_baudrate = B230400;
			break;
		case 460800:
			s_baudrate = B460800;
			break;
		case 500000:
			s_baudrate = B500000;
			break;
		case 576000:
			s_baudrate = B576000;
			break;
		case 921600:
			s_baudrate = B921600;
			break;
		case 1000000:
			s_baudrate = B1000000;
			break;
		case 1152000:
			s_baudrate = B1152000;
			break;
		case 1500000:
			s_baudrate = B1500000;
			break;
		case 2000000:
			s_baudrate = B2000000;
			break;
		case 2500000:
			s_baudrate = B2500000;
			break;
		case 3000000:
			s_baudrate = B3000000;
			break;
		case 3500000:
			s_baudrate = B3500000;
			break;
		case 4000000:
			s_baudrate = B4000000;
			break;
		default:
			s_baudrate = B1152000;
			break;
	}
	serial_set_cflags(s_baudrate | CS8 | CLOCAL | CREAD);
	return;
}


