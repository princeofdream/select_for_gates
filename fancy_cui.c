#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termios.h>
#include "serial_util.h"
#include <sys/time.h>
#include <pthread.h>

#undef DEBUG_JCG
/* #define DEBUG_JCG */

#define DEBUG_FF_ERR


#ifdef DEBUG_JCG
#define JCG(frm,args...) printf("---By James---[%s:%d]--<%s>---"frm"---\n",__FILE__,__LINE__,__func__,##args)
#else
#define JCG(frm,args...)
#endif

struct timeval s_tv,d_tv,m_tv;
int get_sec = 0;
int get_usec = 0;
int get_delta[128];
int get_ff_count = 0;
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
			printf("delta[%d]: %d.\n",i1,get_delta[i1]);
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
		JCG();
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
			printf("got data on stdin: %c\n", c);

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
					get_usec = d_tv.tv_usec - s_tv.tv_usec;
				else
					get_usec = d_tv.tv_sec*1000000 + d_tv.tv_usec - s_tv.tv_usec;

				get_delta[get_ff_count] = get_sec*1000000 + get_usec;
			}
			if(c != 0xFF )
#endif
			{
				gettimeofday(&m_tv,NULL);
				printf("[%d.%d] [%02d] 0x%02x \n", m_tv.tv_sec,m_tv.tv_usec,get_ff_count,c);
			}

#if 0
			if( get_ff_count%8 == 7 )
			{
				printf("----------\n");
				if( get_ff_count%16 == 15 )
					printf("==========\n");
			}
#endif
			get_ff_count++;
			if( get_ff_count > 4096)
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
			printf("got data on stdin: %c\n", c);
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
		/*     printf("\t\t\t\tgot data on serial: 0x%02x\n", c); */
		/* }                                                      */
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldstdtio);
}

int main(int argc, char **argv)
{
	int fd;
	struct termios oldtio;
	char *devname;

	memset(get_delta,0x0,sizeof(get_delta));
	get_ff_count = 0;

	if (argc == 2) {
		devname = argv[1];
	} else {
		printf("usage: %s <serial device>\n", argv[0]);
		return 1;
	}

	if ((fd = serial_open(devname, &oldtio)) < 0) {
		perror(devname);
		return 1;
	}

	JCG();
	main_loop(fd);
	/* main_loop_auto(fd); */
	JCG();

	serial_close(fd, &oldtio);
	return 0;
}
