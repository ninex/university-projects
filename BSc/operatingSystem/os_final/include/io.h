/*
 *   DESCRIPTION: Implementation of x86 I/O instructions
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 30.05.2004
 *     CHANGELOG:
 *                30.05.2004 - Original implementation
 */
#ifndef IO_H
#define IO_H

#ifndef __GNUC__
#define __asm__ asm
#endif

/* ----- Performs a 8-bit output operation to the specified port -------------------------------- */
#define outb(value,  port) \
  __asm__ volatile ("outb %%al, %%dx" \
                    ::"a" (value), "d" ((short)(port)))

/* ----- Performs a 16-bit output operation to the specified port ------------------------------- */
#define outw(value, port) \
  __asm__ volatile ("outw %%ax, %%dx" \
                    ::"a" (value), "d" ((short)(port)))

/* ----- Performs a 32-bit output operation to the specified port ------------------------------- */
#define outl(value, port) \
  __asm__ volatile ("outl %%eax, %%dx" \
                    ::"A" (value), "d" ((short)(port)))

/* ----- Performs a 8-bit input operation from the specified port ------------------------------- */
#define inb(port) ({ \
  unsigned char _v; \
  __asm__ volatile ("inb %%dx, %%al" \
                    :"=a" (_v):"d" ((short)(port))); \
  _v; \
})

/* ----- Performs a 16-bit input operation from the specified port ------------------------------ */
#define inw(port) ({ \
  unsigned short _v; \
  __asm__ volatile ("inw %%dx, %%ax" \
                    :"=a" (_v):"d" ((short)(port))); \
  _v; \
})

/* ----- Performs a 32-bit input operation from the specified port ------------------------------ */
#define inl(port) ({ \
  unsigned int _v; \
  __asm__ volatile ("inl %%dx, %%eax" \
                    :"=A" (_v):"d" ((short)(port))); \
  _v; \
})

/* ----- Writes a 8-bit value to the specified address in memory -------------------------------- */
#define writeb(value, address) \
  __asm__ volatile ("movb %%al, (%%edi)" \
                    ::"a" (value), "D" (address))

/* ----- Writes a 16-bit value to the specified address in memory ------------------------------- */
#define writew(value, address) \
  __asm__ volatile ("movw %%ax, (%%edi)" \
                    ::"a" (value), "D" (address))

/* ----- Writes a 32-bit value to the specified address in memory ------------------------------- */
#define writel(value, address) \
  __asm__ volatile ("movl %%eax, (%%edi)" \
                    ::"A" (value), "D" (address))

/* ----- Reads a 8-bit value from the specified address in memory -------------------------------- */
#define readb(address) ({ \
  unsigned char _v; \
  __asm__ volatile ("movb (%%edi), %%al" \
                    :"=a" (_v):"D" (address)); \
  _v; \
})

/* ----- Reads a 16-bit value from the specified address in memory ------------------------------- */
#define readw(address) ({ \
  unsigned short _v; \
  __asm__ volatile ("movw (%%edi), %%ax" \
                    :"=a" (_v):"D" (address)); \
  _v; \
})

/* ----- Reads a 32-bit value from the specified address in memory ------------------------------- */
#define readl(address) ({ \
  unsigned int _v; \
  __asm__ volatile ("movl (%%edi), %%eax" \
                    :"=A" (_v):"D" (address)); \
  _v; \
})

#endif
