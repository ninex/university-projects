#ifndef TRACE_H
#define TRACE_H

/* ----- Standard CGA/EGA/VGA/MCGA background attributes in text mode --------------------------- */
#define ATTR_BG_BLACK      0x00
#define ATTR_BG_BLUE       0x10
#define ATTR_BG_GREEN      0x20
#define ATTR_BG_CYAN       0x30
#define ATTR_BG_RED        0x40
#define ATTR_BG_MAGENTA    0x50
#define ATTR_BG_BROWN      0x60
#define ATTR_BG_LIGHT_GREY 0x70

/*
 * Standard CGA/EGA/VGA/MCGA foreground attributes in text mode. Note that on MDA adapaters
 * the attributes are fixed
 */
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

/* ----- Display definitions for BIOS mode 3 (80x25 alphanumeric mode) -------------------------- */
#define MAX_COLUMNS  80
#define MAX_ROWS     25
#define DISPLAY_SIZE (MAX_COLUMNS*MAX_ROWS*2)
#define LINE_WIDTH   (MAX_COLUMNS*2)

void (*write_string)(char *s);

/*
 * DESCRIPTION: The printk function is used to output a NULL terminated character
 *              string to the display. The string may contain format
 *              specifiers, length modifiers and escape sequences similar to
 *              the printf statement.
 *       INPUT: format - A character string containing format specifiers, etc.
 *              Additional parameters (1 per format specifier) must be included.
 *
 *              The following specifiers are supported:
 *              %c - display a single character
 *              %d - display an integer in decimal
 *              %i - display an integer in decimal
 *              %o - display an integer value in octal
 *              %s - display a character string
 *              %u - display an unsigned integer value in decimal
 *              %x, %X - display an integer in hexadecimal
 *
 *              The following escape sequences are supported:
 *              \n - newline
 *
 *              The following length modifiers are supported:
 *                  h - The next parameter is interpreted as either
 *                      a signed or unsigned short
 *                 hh - The next parameter is interpreted as either
 *                      a signed or unsigned char
 *              ll, L - The next parameter is interpreted as either
 *                      a signed or unsigned long long (64-bit value for 32-bit architectures)
 *
 *              Special characters:
 *              # - For 'o' specifiers a 0 is written before the value
 *                  For 'x' specifiers the string '0x' is written before the value
 *                  For 'X' specifiers the string '0X' is written before the value
 *              . - Specifies the field width. Empty spaces are zero-padded.
 *                  Values are not truncated if the specified width is less than
 *                  the space required by a field.
 *      OUTPUT: Writes the designated string to the display according to the
 *              format specifiers starting at the current display position using the
 *              current display attribute. The physical location of the cursor is also
 *              updated.
 *    EXAMPLES: printk("x is equal to %.4d\n", 3); produces the output
 *              'x is equal to 0003'
 */
void printk(char *format, ...);

/*
 * DESCRIPTION: Sets the display attribute that will be used by the
 *              printk function when writing to the display
 *       INPUT: foreground - Foreground attribute
 *              background - Background attribute
 *      OUTPUT: NONE
 */
void (*set_attribute)(unsigned char foreground, unsigned char background);

/*
 * DESCRIPTION: Returns the current display attribute (both the foreground and
 *              and background attributes)
 *       INPUT: NONE
 *      OUTPUT: Returns the current display attribute
 */
unsigned char (*get_attribute)();

/*
 * DESCRIPTION: Returns the current offset relative to the display base address
 *       INPUT: NONE
 *      OUTPUT: Returns the current display offset
 */
int trace_get_offset();

/*
 * DESCRIPTION: Updates the display position and physical cursor location. The next
 *              output written to the display will start at the designated position.
 *       INPUT: x - Display column
 *              y - Display row
 *      OUTPUT: NONE
 */
void (*gotoxy)(int x, int y);

/*
 * DESCRIPTION: The display is cleared using the current attribute settings.
 *              The display position is reset to the origin as if gotoxy(0, 0)
 *              was executed.
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void (*clear_display)();

/*
 * DESCRIPTION: Initialize the trace module. This function must be called
 *              before any of the other functions are used.
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void trace_initialize();

#endif
