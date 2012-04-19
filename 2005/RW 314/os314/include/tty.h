#ifndef TTY_H
#define TTY_H

int active();
int tty_open();
void tty_close(int handle);
void tty_print(int handle, char *s);
void tty_set_attribute(int handle, unsigned char foreground, unsigned char background);
unsigned char tty_get_attribute(int handle);
void tty_clear_display(int handle);
void tty_gotoxy(int handle, int x, int y);
unsigned char tty_key_available(int handle);
unsigned char tty_read_key(int handle);
void tty_set_kernel_focus();
void tty_initialize();

#endif
