/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  test
 *
 *        Version:  1.0
 *        Created:  2018年09月06日 19时21分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  James Lee (JamesL), princeofdream@outlook.com
 *   Organization:  BookCL
 *
 * =====================================================================================
 */


#include "fancy_cui.h"
#include "serial_util.h"
#include <stdio.h>


int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: %s <serial device> [115200|9600...]\n", argv[0]);
		return -1;
	}
	printf("-->%s<--\n",argv[1]);
	fancy_cui_main(argv[1], 115200);
	return 0;
}

