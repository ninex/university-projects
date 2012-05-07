#ifndef I8253_H
#define I8253_H

/* ----- Time slice in milliseconds ------------------------------------------------------------- */
#define QUANTUM 20
/* ----- Time is measured in milliseconds ------------------------------------------------------- */
#define TIME_UNIT 1000
/* ----- The rate (Hz) at which the counter registers are adjusted ------------------------------ */
#define RATE 1193180
/* ----- Counter register value to obtain an interrupt after QUANTUM ms ------------------------- */
#define N ((RATE*QUANTUM)/TIME_UNIT)

/*
 * DESCRIPTION: Initializes the system clock (Intel 8253/8254 timer chip)
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void timer_initialize();

/*
 * DESCRIPTION: Set a timer to expire within a specified amount of
 *              microseconds. The maximum timeout that can be specified
 *              is 10ms.
 *       INPUT: us - Number of microseconds before the time expires
 *      OUTPUT: Returns a handle that can be used with the 'timer_expired'
 *              function to determine if a timeout occurred.
 */
int timer_set(int us);

/*
 * DESCRIPTION: Test if a timer previously set with 'timer_set' has expired
 *       INPUT: handle - Timer handle obtained from 'timer_set'
 *      OUTPUT: Returns 1 if the timer has expired, 0 otherwise
 */
unsigned char timer_expired(int handle);

/*
 * DESCRIPTION: Returns the total amount of elapsed clock ticks. The count is only adjusted
 *              if a timer interrupt (IRQ0) occurs
 *       INPUT: NONE
 *      OUTPUT: Returns the number of clock ticks
 */
int timer_get_ticks();

#endif
