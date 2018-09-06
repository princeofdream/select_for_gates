/*
 * =====================================================================================
 *
 *       Filename:  log_util.h
 *
 *    Description:  log util
 *
 *        Version:  1.0
 *        Created:  2017年05月18日 09时22分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (Jsl), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termios.h>
#include "serial_util.h"
#include <sys/time.h>

#include <time.h>


char* get_log_path();
void set_log_path(char* path);
int exit_log_util();
int init_log_util(char* path);
void set_log_to_file(int mode);


void logd(char* log_str);

char lstr[4096];



