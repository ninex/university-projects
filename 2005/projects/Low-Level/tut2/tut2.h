#ifndef TUT2_H
#define TUT2_H

/*
 *       INPUT: int n
 *      OUTPUT: int
 * DESCRIPTION: Returns n!
 */
extern int fact(int n);

extern int ackerman(int x, int y);

/*
 *       INPUT: int x
 *              int y
 *      OUTPUT: int
 * DESCRIPTION: Returns x^y
 */
extern int power(int x, int y);

/*
 *       INPUT: int *x
 *              int *y
 *      OUTPUT: none
 * DESCRIPTION: Exchanges the contents of the variables pointed to
 *              by x and y
 */
extern void swap(int *x, int *y);

/*
 *       INPUT: int n - number to located
 *              int list[] - a sorted list of integers
 *              int low - lower list boundary
 *              int high - upper list boundary
 *      OUTPUT: int
 * DESCRIPTION: Returns the position inside 'list' containing
 *              the value 'n'. Returns -1 if 'n' could not be located
 */
extern int binary_search(int n, int list[], int low, int high);

#endif
