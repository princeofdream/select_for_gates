/*
 * =====================================================================================
 *
 *       Filename:  log_util.c
 *
 *    Description:  log util
 *
 *        Version:  1.0
 *        Created:  2017年05月18日 09时22分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (Jsl), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */

#include "log_util.h"

static int fd = -1;
char log_path[4096];


static int log_to_file = 0;

char* get_log_path()
{
}

void set_log_path(char* path)
{
}


int init_log_util(char* path)
{
	struct tm *m_tm;
	time_t m_time;

	memset(log_path,0x0,sizeof(log_path));
	time(&m_time);
	m_tm = localtime(&m_time);

	if (log_to_file <= 0) {
		return 0;
	}

	if(path == NULL)
	{
		sprintf(log_path,"%d-%d-%d_%d%d%d.log", \
				m_tm->tm_year+1900, m_tm->tm_mon+1,m_tm->tm_mday, \
				m_tm->tm_hour,m_tm->tm_min,m_tm->tm_sec);
	}
	else
	{
		sprintf(log_path,"%d-%d-%d_%d%d%d-%s.log", \
				m_tm->tm_year+1900, m_tm->tm_mon+1,m_tm->tm_mday, \
				m_tm->tm_hour,m_tm->tm_min,m_tm->tm_sec,\
				path);
	}
	printf("Path: %s\n",log_path);
	if(fd > 0)
	{
		close(fd);
		fd=-1;
	}
	fd=open(log_path,O_RDWR|O_CREAT|O_APPEND,0664);
	if( fd < 0 )
	{
		printf("Write Log file Fail!");
	}
}

int exit_log_util()
{
	close(fd);
}


void logd(char* log_str)
{
	if(log_to_file > 0 && fd < 0)
	{
		init_log_util(NULL);
	}

	printf("%s",log_str);
	//force output
	fflush(stdout);
	if (log_to_file > 0) {
		write(fd,log_str,strlen(log_str));
	}
}

void set_log_to_file(int mode)
{
	log_to_file = mode;
}


