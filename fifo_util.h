/*
 * =====================================================================================
 *
 *       Filename:  fifo_util.h
 *
 *    Description:  fifo util
 *
 *        Version:  1.0
 *        Created:  2017年05月18日 10时46分58秒
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


#define UART_EMU_FIFO "uart_emu_fifo"
#define USE_UART_EMU


void init_uart_emu_fifo_util();


