#ifndef _SERIAL_UTIL_H
#define _SERIAL_UTIL_H

#include <termios.h>

extern int serial_open(const char *fn, struct termios *tio_saved);
extern void serial_close(int fd, const struct termios *tio_saved);
void serial_set_cflags(tcflag_t flag);
tcflag_t serial_get_cflags();
void serial_set_baudrate(int rate);

#endif	/* _SERIAL_UTIL_H */
