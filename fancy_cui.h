/*
 * =====================================================================================
 *
 *       Filename:  fancy_cui.h
 *
 *    Description:  fancy cui header file
 *
 *        Version:  1.0
 *        Created:  2017年05月18日 10时56分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (Jsl), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */

#ifndef __FANCY_CUI_HEADER__
#define __FANCY_CUI_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termios.h>
#include <sys/time.h>
#include <pthread.h>

#include "serial_util.h"
#include "log_util.h"
#include "fifo_util.h"

int	fancy_cui_main(char* dev_path, int rate);
int serial_terminal(char* dev_path, int rate, tcflag_t flag, int log_to_file, int emu_serial);

#endif /* ifndef __FANCY_CUI_HEADER__ */
