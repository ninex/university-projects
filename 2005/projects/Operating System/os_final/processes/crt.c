#include <stdarg.h>
#include "include/sys_calls.h"

/* ----- Flags used to track conversion specifiers and length modifiers ------------------------- */
#define FL_UPPER   0x01
#define FL_SIGN    0x02
#define FL_INT     0x04
#define FL_SPECIAL 0x08
#define FL_WIDTH   0x10

/* ----- Size of the internal buffer used to process format specifiers -------------------------- */
#define PRINTF_BUFFER_SIZE 1024

/*
 * Macro to perform 64-bit division (include/asm-i386/div64.h - Linux kernel)
 */
#define div_64(n, base) ({ \
  unsigned long __upper; \
  unsigned long __low; \
  unsigned long __high; \
  unsigned long __mod; \
  __asm__("":"=a" (__low), "=d" (__high):"A" (n)); \
  __upper = __high; \
  if (__high) { \
    __upper = __high % (base); \
    __high = __high / (base); \
  } \
  __asm__("divl %2":"=a" (__low), "=d" (__mod):"rm" (base), "0" (__low), "1" (__upper)); \
  __asm__("":"=A" (n):"a" (__low),"d" (__high)); \
  __mod; \
})

/* ----- Used by %x conversion specifier -------------------------------------------------------- */
char small_digits[] = "0123456789abcdef";

/* ----- Used by %X conversion specifier -------------------------------------------------------- */
char large_digits[] = "0123456789ABCDEF";

/* ----- Intermediate buffer used by crt_printf ------------------------------------------------- */
char buffer[PRINTF_BUFFER_SIZE];

int int2string(long long x, char *s, int base, int flags, int width) {
  int i, j;
  char tmp[32];
  char sign;

  i = 0;
  sign = 0;
  if ((x < 0) && ((flags & FL_SIGN) != 0)) {
    x = -x;
    sign = '-';
  }

  if ((flags & FL_UPPER) != 0) {
    do {
      tmp[i] = large_digits[div_64(x, base)];
      i++;
      width--;
    } while (x != 0);
  } else {
    do {
      tmp[i] = small_digits[div_64(x, base)];
      i++;
      width--;
    } while (x != 0);
  }
  i--;
  j = 0;
  if ((flags & FL_SPECIAL) != 0) {
    if (base == 8) {
      s[j] = '0';
      j++;
    } else if (base == 16) {
      s[j] = '0';
      j++;
      if ((flags & FL_UPPER) != 0)
        s[j] = 'X';
      else
        s[j] = 'x';
      j++;
    }
  }

  if (((flags & FL_SIGN) != 0) && (sign != 0)) {
    s[j] = sign;
    j++;
  }

  if ((flags & FL_WIDTH) != 0) {
    while (width > 0) {
      s[j] = '0';
      j++;
      width--;
    }
  }

  do {
    s[j] = tmp[i];
    j++;
    i--;
  } while (i >= 0);
  s[j] = 0;

  return j;
}


int crt_open() {
  return _sys_0(SYS_TTY_OPEN);
}

void crt_close(int handle) {
  sys_1(SYS_TTY_CLOSE, handle);
}

void crt_gotoxy(int handle, int x, int y) {
  sys_3(SYS_TTY_GOTOXY, handle, x, y);
}

void crt_set_attribute(int handle, unsigned char foreground, unsigned char background) {
  sys_3(SYS_TTY_SET_ATTRIBUTE, handle, foreground, background);
}

unsigned char crt_get_attribute(int handle) {
  return _sys_1(SYS_TTY_GET_ATTRIBUTE, handle);
}

void crt_clear_display(int handle) {
  sys_1(SYS_TTY_CLEAR_DISPLAY, handle);
}

void crt_printf(int handle, char *format, ...) {
  va_list ap;
  int base;
  int flags;
  int width;
  int i;
  char *c;
  char *s;
  char ch;
  char length;
  unsigned long long value;

  va_start(ap, format);
  c = format;
  i = 0;
  while (*c != 0) {
    base = 10;
    flags = 0;
    value = 0;
    length = 0;
    width = 0;

    if (*c == '%') {
      c++;
      /* ----- Special characters --------------------------------------------------------------- */
      if (*c == '#') {
        flags = flags | FL_SPECIAL;
        c++;
      }
      /* ----- Field width ---------------------------------------------------------------------- */
      if (*c == '.') {
        c++;
        flags = flags | FL_WIDTH;
        while ((*c >= '0') && (*c <= '9')) {
          width = (width*10)+(*c-'0');
          c++;
        }
      }
      /* ----- Length modifiers ----------------------------------------------------------------- */
      if ((*c == 'h') || (*c == 'l') || (*c == 'L')) {
        length = *c;
        c++;
        if ((*c == 'l') && (length == 'l')) {
          length = 'L';
          c++;
        } else if ((*c == 'h') && (length == 'h')) {
          length = 'H';
          c++;
        }
      }
      /* ----- Conversion specifiers ------------------------------------------------------------ */
      switch (*c) {
        case 'c':
          ch = (unsigned char)(va_arg(ap, int));
          buffer[i] = ch;
          i++;
          break;
        case 'd':
        case 'i':
          flags = flags | FL_SIGN | FL_INT;
          break;
        case 'o':
          flags = flags | FL_INT;
          base = 8;
          break;
        case 's':
          s = (char *)(va_arg(ap, int));
          while (*s != 0) {
            buffer[i] = *s;
            i++;
            s++;
          }
          break;
        case 'u':
          flags = flags | FL_INT;
          break;
        case 'X':
          flags = flags | FL_UPPER | FL_INT;
          base = 16;
          break;
        case 'x':
          flags = flags | FL_INT;
          base = 16;
          break;
        default:
          buffer[i] = *c;
          i++;
          break;
      }
      /* ----- Cast the next integer argument to an appropriate type ---------------------------- */
      if ((flags & FL_INT) != 0) {
        if (length == 'L')
          value = va_arg(ap, long long);
        else if (length == 'l') {
          value = va_arg(ap, unsigned long);
          if ((flags & FL_SIGN) != 0)
            value = (signed long) value;
        } else if (length == 'h') {
          value = (unsigned short) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed short) value;
        } else if (length == 'H') {
          value = (unsigned char) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed char) value;
        } else {
          value = (unsigned int) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed int) value;
        }
        /* ----- Convert the number into a string ----------------------------------------------- */
        i = i+int2string(value, &buffer[i], base, flags, width);
      }
    } else {
      buffer[i] = *c;
      i++;
    }
    c++;
  }

  buffer[i] = '\0';
  va_end(ap);
  sys_2(SYS_TTY_PRINT, handle, buffer);
}

unsigned char crt_key_available(int handle) {
  return _sys_1(SYS_TTY_KEY_AVAILABLE, handle);
}

unsigned char crt_read_key(int handle) {
  return _sys_1(SYS_TTY_READ_KEY, handle);
}
