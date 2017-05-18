BIN	:= fancy_cui
OBJS	:= fancy_cui.o serial_util.o log_util.o fifo_util.o
CFLAGS	:= -O2 -Wall -Wextra -static -lpthread
# CC	:= gcc
# CC	:= arm-linux-gnueabihf-gcc -static
# CC	:= arm-fsl-linux-gnueabi-gcc -static
# CC	:= arm-openwrt-linux-gcc

LDFLAGS= -pthread -lpthread

all: $(BIN)

clean:
	- rm $(BIN) *.o

fancy_cui: $(OBJS)

fancy_cui.o: \
	fancy_cui.c serial_util.h
serial_util.o: \
	serial_util.c
log_util.o: \
	log_util.c

fifo_util.o : \
	fifo_util.c

