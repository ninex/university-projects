#ifndef TUT1_H
#define TUT1_H

/*
 *       INPUT: int x
 *              int y
 *      OUTPUT: int
 * DESCRIPTION: Compares 'x' and 'y' and returns the
 *              largest value. If the two value are
 *              equivalent, then either 'x' or 'y' may
 *              be returned
 */
extern int max(int x, int y);

/*
 *       INPUT: int x (x >= 0)
 *              int y (y >= 0)
 *      OUTPUT: int
 * DESCRIPTION: Returns x^y
 */
extern int power(int x, int y);

/*
 *       INPUT: int m (m >= 0)
 *              int n (n >= 0)
 *      OUTPUT: int
 * DESCRIPTION: Computes the greatest common divisor of
 *              m and n using Euclid's algorithm
 */
extern int gcd(int m, int n);

#endif
