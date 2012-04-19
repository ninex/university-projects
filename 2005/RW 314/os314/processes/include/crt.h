#ifndef CRT_H
#define CRT_H

#include <stdarg.h>

/*
 * Standard CGA/EGA/VGA/MCGA background attributes in text mode
 */
#define ATTR_BG_BLACK      0x00
#define ATTR_BG_BLUE       0x10
#define ATTR_BG_GREEN      0x20
#define ATTR_BG_CYAN       0x30
#define ATTR_BG_RED        0x40
#define ATTR_BG_MAGENTA    0x50
#define ATTR_BG_BROWN      0x60
#define ATTR_BG_LIGHT_GREY 0x70

/* ----- Standard CGA/EGA/VGA/MCGA foreground attributes in text mode --------------------------- */
#define ATTR_FG_BLACK         0x00
#define ATTR_FG_BLUE          0x01
#define ATTR_FG_GREEN         0x02
#define ATTR_FG_CYAN          0x03
#define ATTR_FG_RED           0x04
#define ATTR_FG_MAGENTA       0x05
#define ATTR_FG_BROWN         0x06
#define ATTR_FG_LIGHT_GREY    0x07
#define ATTR_FG_DARK_GREY     0x08
#define ATTR_FG_LIGHT_BLUE    0x09
#define ATTR_FG_LIGHT_GREEN   0x0a
#define ATTR_FG_LIGHT_CYAN    0x0b
#define ATTR_FG_LIGHT_RED     0x0c
#define ATTR_FG_LIGHT_MAGENTA 0x0d
#define ATTR_FG_YELLOW        0x0e
#define ATTR_FG_WHITE         0x0f

/* ----- The most significant bit of the background attribute allows for blinking text ---------- */
#define ATTR_BLINK 0x80

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
int crt_open();

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void crt_close(int handle);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void crt_gotoxy(int handle, int x, int y);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void crt_set_attribute(int handle, unsigned char foreground, unsigned char background);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
unsigned char crt_get_attribute(int handle);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void crt_clear_display(int handle);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void crt_printf(int handle, char *format, ...);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
unsigned char crt_key_available(int handle);

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
unsigned char crt_read_key(int handle);

#endif
