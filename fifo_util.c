/*
 * =====================================================================================
 *
 *       Filename:  fifo_util.c
 *
 *    Description:  fifo util
 *
 *        Version:  1.0
 *        Created:  2017年05月18日 10时46分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (Jsl), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */


#include "fifo_util.h"



void init_uart_emu_fifo_util()
{
#ifdef USE_UART_EMU
	unlink(UART_EMU_FIFO);
	mkfifo(UART_EMU_FIFO,O_RDWR,0644);
	chmod(UART_EMU_FIFO,00664);
	printf("Make fifo file!\n");
#endif
}



