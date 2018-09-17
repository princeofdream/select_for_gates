BIN	:= fancy_cui
OBJS := fancy_cui.o serial_util.o log_util.o fifo_util.o


ARCH ?= x86

ifeq (${ARCH}, arm)
# CC := arm-linux-gnueabihf-gcc -static
# CC	:= arm-fsl-linux-gnueabi-gcc -static
# CC	:= arm-openwrt-linux-gcc
CC := arm-linux-androideabi-gcc
else
CC := gcc
endif

ifeq (${ARCH}, arm)
CFLAGS = -static -fPIE
LDFLAGS= -fPIE

LIBRARY_CFLAGS  := $(CFLAGS)
LIBRARY_CFLAGS  += -DBUILD_FOR_LIBRARY -fPIC
LIBRARY_LDFLAGS := $(LDFLAGS)
else
CFLAGS	:= -O2 -Wall -Wextra -lpthread -fPIC
CFLAGS += -DUSE_UART_EMU
LDFLAGS= -pthread -lpthread

LIBRARY_CFLAGS  := $(CFLAGS)
LIBRARY_CFLAGS  += -DBUILD_FOR_LIBRARY -fPIC
LIBRARY_LDFLAGS := $(LDFLAGS)
LIBRARY_LDFLAGS += -shared
endif



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
	@rm -rf $(BIN) *.so *.o test uart_emu_fifo*

distclean: clean
	rm -rf test uart_emu_fifo*
