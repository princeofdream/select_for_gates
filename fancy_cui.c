
#include "fancy_cui.h"

#undef DEBUG_JCG
/* #define DEBUG_JCG */
/* #define DEBUG_FF_ERR */


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


void* pthread_print_data(void* data)
{
#ifdef DEBUG_FF_ERR
	int i1=0;
	sleep(1);
	while(i1 < 128)
	{
		if(get_delta[i1] == 0)
			break;
		if(get_delta[i1+1] == 0)
		{
			memset(lstr,0x0,sizeof(lstr));
			sprintf(lstr,"delta[%d]: %d.\n",i1,get_delta[i1]);
			logd(lstr);
		}
		i1++;
	}
	memset(get_delta,0x0,sizeof(get_delta));
	get_ff_count=0;
#endif
}

static void main_loop(int fd)
{
	struct termios newstdtio, oldstdtio;
	fd_set readfds;
	unsigned char c;
	ssize_t rsize;

	tcgetattr(STDIN_FILENO, &oldstdtio);
	memcpy(&newstdtio, &oldstdtio, sizeof(struct termios));
	newstdtio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newstdtio);

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(fd, &readfds);

	for (;;) {
		fd_set rfds = readfds;
		int sel_result;

		JCG();
		sel_result = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
		if (sel_result < 0) {
			perror("select");
			break;
		} else if (sel_result == 0) {
			/* timeout: no way */
		} else if (FD_ISSET(STDIN_FILENO, &rfds)) {
			int i1=0;
			rsize = read(STDIN_FILENO, &c, 1);
			if (rsize < 0) {
				perror("read");
				break;
			}
			memset(lstr,0x0,sizeof(lstr));
			sprintf(lstr,"got data on stdin: %c\n", c);
			logd(lstr);

			if (c == 'q')
				break;
			write(fd, &c, 1);
			pthread_create(&m_pid, NULL, pthread_print_data, NULL);
		} else if (FD_ISSET(fd, &rfds)) {
			get_c=c;
			rsize = read(fd, &c, 1);

			if (rsize < 0) {
				perror("read");
				break;
			}

#ifdef DEBUG_FF_ERR
			if( c == 0xFF )
			{
				if(get_c != 0xFF)
				{
					gettimeofday(&s_tv,NULL);
				}
				gettimeofday(&d_tv,NULL);

				get_sec = d_tv.tv_sec - s_tv.tv_sec;
				if(d_tv.tv_usec > s_tv.tv_usec)
					get_usec = get_sec*1000000 + d_tv.tv_usec - s_tv.tv_usec;
				else
					get_usec = get_sec*1000000 + d_tv.tv_usec - s_tv.tv_usec;

				get_delta[get_ff_count] = get_usec;
				/* memset(lstr,0x0,sizeof(lstr)); */
				/* sprintf(lstr,"0x%02x\n",c); */
				/* logd(lstr); */
				/* gettimeofday(&m_tv,NULL);                                                    */
				/* memset(lstr,0x0,sizeof(lstr)); */
				/* sprintf(lstr,"[%d.%d] [%02d] 0x%02x \n", m_tv.tv_sec,m_tv.tv_usec,get_ff_count,c); */
				/* logd(lstr); */
			}
			if(c != 0xFF )
#endif
			{
				gettimeofday(&m_tv,NULL);
				memset(lstr,0x0,sizeof(lstr));
				/* sprintf(lstr,"[%d.%06d] [%02d] 0x%02x \n", m_tv.tv_sec,m_tv.tv_usec,get_ff_count,c); */
				if(get_ff_count %8 ==7)
				{
					if(get_ff_count %16 == 15)
						sprintf(lstr,"0x%02x\n", c);
					else
						sprintf(lstr,"0x%02x \t", c);
				}
				else
					sprintf(lstr,"0x%02x ", c);
				// print stdout data
				logd(lstr);
			}

			get_ff_count++;
			if( get_ff_count >= 1000000000 )
				get_ff_count = 0;

		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldstdtio);
}

static void main_loop_auto(int fd)
{
	struct termios newstdtio, oldstdtio;
	fd_set readfds;
	unsigned char c;
	ssize_t rsize;

	tcgetattr(STDIN_FILENO, &oldstdtio);
	memcpy(&newstdtio, &oldstdtio, sizeof(struct termios));
	newstdtio.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newstdtio);

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(fd, &readfds);

	for (;;) {
		fd_set rfds = readfds;
		int sel_result;

		JCG();
		/* sel_result = select(FD_SETSIZE, &rfds, NULL, NULL, NULL); */
		/* JCG(); */
		/* if (sel_result < 0) {                       */
		/*     perror("select");                       */
		/*     break;                                  */
		/* } else if (sel_result == 0) {               */
		/*     [> timeout: no way <]                   */
		/* } else if (FD_ISSET(STDIN_FILENO, &rfds)) { */
			JCG();
			sleep(1);
			/* rsize = read(STDIN_FILENO, &c, 1); */
			/* JCG();                             */
			/* if (rsize < 0) {                   */
			/*     perror("read");                */
			/*     break;                         */
			/* }                                  */
			if(c > 0x70)
				c=0x61;
			memset(lstr,0x0,sizeof(lstr));
			sprintf(lstr,"got data on stdin: %c\n", c);
			logd(lstr);
			if (c == 'q')
				break;
			JCG();
			write(fd, &c, 1);
			JCG();
			c++;
		/* } else if (FD_ISSET(fd, &rfds)) {                      */
		/*     JCG();                                             */
		/*     rsize = read(fd, &c, 1);                           */
		/*     JCG();                                             */
		/*     if (rsize < 0) {                                   */
		/*         perror("read");                                */
		/*         break;                                         */
		/*     }                                                  */
			/* memset(lstr,0x0,sizeof(lstr)); */
		/*     sprintf(lstr,"\t\t\t\tgot data on serial: 0x%02x\n", c); */
			/* logd(lstr); */
		/* }                                                      */
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldstdtio);
}

#ifndef BUILD_FOR_LIBRARY
int main(int argc, char **argv)
{
	char *devname;
	int baudrate = 0;
	int emu=0;
	int i0 = 0;

	if (argc >= 2) {
	} else {
		printf("usage: %s <serial device> [115200|9600...] [fifo_emu]\n", argv[0]);
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
			default:
				break;
		}
		i0++;
	}
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
