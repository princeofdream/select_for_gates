
#include "fancy_cui.h"

#undef DEBUG_JCG
#define DEBUG_JCG


#ifdef DEBUG_JCG
#define JCG(frm,args...) printf("---By James---[%s:%d]--<%s>---"frm"---\n",__FILE__,__LINE__,__func__,##args)
#else
#define JCG(frm,args...)
#endif

struct timeval s_tv,d_tv,m_tv;
int get_sec = 0;
int get_usec = 0;
int get_delta[128];
unsigned long get_ff_count = 0;
unsigned char get_c=0;
pthread_t m_pid;
static int input_fd = STDIN_FILENO;
static int log_input = 0;

static void main_loop(int fd)
{
	struct termios newstdtio, oldstdtio;
	fd_set readfds;
	unsigned char c;
	ssize_t rsize;

	tcgetattr(input_fd, &oldstdtio);
	memcpy(&newstdtio, &oldstdtio, sizeof(struct termios));
	newstdtio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(input_fd, TCSANOW, &newstdtio);

	FD_ZERO(&readfds);
	FD_SET(input_fd, &readfds);
	FD_SET(fd, &readfds);

	for (;;) {
		fd_set rfds = readfds;
		int sel_result;

		sel_result = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
		if (sel_result < 0) {
			perror("select");
			break;
		} else if (sel_result == 0) {
			/* timeout: no way */
		} else if (FD_ISSET(input_fd, &rfds)) {
			int i1=0;
			rsize = read(input_fd, &c, 1);
			if (rsize < 0) {
				perror("read");
				break;
			}
			memset(lstr,0x0,sizeof(lstr));
			if (log_input > 0 && log_input <0xff) {
				sprintf(lstr,"%c", c);
				logd(lstr);
			} else if (log_input >= 0xff) {
				sprintf(lstr,"\ninput: < %c >, return:", c);
				logd(lstr);
			}

			if (c == 'q')
				break;
			write(fd, &c, 1);
		} else if (FD_ISSET(fd, &rfds)) {
			get_c=c;
			rsize = read(fd, &c, 1);

			if (rsize < 0) {
				perror("read");
				break;
			}

			/* gettimeofday(&m_tv,NULL); */
			memset(lstr,0x0,sizeof(lstr));
			sprintf(lstr,"%c", c);
			logd(lstr);

			get_ff_count++;
			if( get_ff_count >= 1000000000 )
				get_ff_count = 0;

		}
	}

	tcsetattr(input_fd, TCSANOW, &oldstdtio);
}

static void main_loop_auto(int fd)
{
	struct termios newstdtio, oldstdtio;
	fd_set readfds;
	unsigned char c;
	ssize_t rsize;

	tcgetattr(input_fd, &oldstdtio);
	memcpy(&newstdtio, &oldstdtio, sizeof(struct termios));
	newstdtio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(input_fd, TCSANOW, &newstdtio);

	FD_ZERO(&readfds);
	FD_SET(input_fd, &readfds);
	FD_SET(fd, &readfds);

	for (;;) {
		fd_set rfds = readfds;
		int sel_result;

			sleep(1);
			if(c > 0x70)
				c=0x61;
			memset(lstr,0x0,sizeof(lstr));
			sprintf(lstr,"got data on stdin: %c\n", c);
			logd(lstr);
			if (c == 'q')
				break;
			write(fd, &c, 1);
			c++;
	}

	tcsetattr(input_fd, TCSANOW, &oldstdtio);
}

#ifndef BUILD_FOR_LIBRARY
int main(int argc, char **argv)
{
	char *devname;
	int baudrate  = 0;
	int emu       = 1;
	int i0        = 0;
	int log_input = 0x1FF;

	if (argc >= 2) {
	} else {
		printf("usage: %s <serial device> [115200|9600...]\n", argv[0]);
		return 1;
	}
	while (i0 < argc) {
		switch (i0) {
			case 1:
				devname = argv[i0];
				break;
			case 2:
				baudrate = atoi(argv[i0]);
				break;
			case 3:
				emu = atoi(argv[i0]);
				break;
			case 4:
				log_input = atoi(argv[i0]);
				break;
			default:
				break;
		}
		i0++;
	}
	set_log_input(log_input);
	serial_terminal(devname, baudrate,0,0,emu);
	return 0;
}
#endif

int	fancy_cui_main(char* dev_path, int rate)
{
	int ret;
	ret = serial_terminal(dev_path, rate, 0, 0, 0);
	return ret;
}

int serial_terminal(char* dev_path, int rate, tcflag_t flag, int log_to_file, int emu_serial)
{
	int fd;
	struct termios oldtio;
	int baudrate = rate;

	memset(get_delta,0x0,sizeof(get_delta));
	get_ff_count = 0;

	serial_set_baudrate(baudrate);
	if ((fd = serial_open(dev_path, &oldtio)) < 0) {
		perror(dev_path);
		return 1;
	}

	init_log_util(NULL);
	set_log_to_file(log_to_file);

#ifdef USE_UART_EMU
	if (emu_serial) {
		init_uart_emu_fifo_util();
		close(fd);
		fd=open(UART_EMU_FIFO,O_RDWR);
	}
#endif

	main_loop(fd);
	/* main_loop_auto(fd); */

	serial_close(fd, &oldtio);
	exit_log_util();
	return 0;
}

void set_input_fd(int fd)
{
	input_fd = fd;
}

void set_log_input(int mode)
{
	log_input = mode;
}

