BIN	:= fancy_cui
OBJS	:= fancy_cui.o serial_util.o log_util.o fifo_util.o
CFLAGS	:= -O2 -Wall -Wextra -lpthread -fPIC
# CFLAGS  += -static
# CC	:= gcc
# CC	:= arm-linux-gnueabihf-gcc -static
# CC	:= arm-fsl-linux-gnueabi-gcc -static
# CC	:= arm-openwrt-linux-gcc

LDFLAGS= -pthread -lpthread

LIBRARY_CFLAGS  := $(CFLAGS)
LIBRARY_CFLAGS  += -DBUILD_FOR_LIBRARY -fPIC
LIBRARY_LDFLAGS := $(LDFLAGS)
LIBRARY_LDFLAGS += -shared


.PHONY: clean

all: $(BIN) libfancycui

fancy_cui: $(OBJS)
	set -e;for i in $(OBJS); do $(CC) $(CFLAGS) -c $(LDFLAGS) $$i;done
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS)  -o $(@)

libfancycui: $(OBJS)
	set -e;for i in $(OBJS); do $(CC) $(LIBRARY_CFLAGS) -c $(LIBRARY_LDFLAGS) $$i;done
	$(CC) $(LIBRARY_CFLAGS) $(LIBRARY_LDFLAGS) $(OBJS) -o $(@).so

test: test.c libfancycui
	$(CC) $(CFLAGS) $(LDFLAGS) test.c -lfancycui -o test -L. -Wl,-rpath=.

clean:
	@rm $(BIN) *.so *.o test* uart_emu_fifo*

distclean: clean
	rm -rf test* uart_emu_fifo*
