#ifndef FRACTIONSQ1_H
#define FRACTIONSQ1_H
/*! ============================================================================

COMPONENT:                  F R A C T I O N S Q 1 . H 
AUTHOR: J.J.D. Bull
DESCRIPTION: Provides operations to create, dispose, add, subtract, multiply,
						 and divide fractions. There are three conversion routines: string
						 to fraction, fraction to string and fraction to real. Fraction is 
						 an opaque type. Fractions are always stored and processed in 
						 reduced form.  


VERSION:          1
DATED:            2004/02/12 
LAST MODIFIED BY: jbull

============================================================================= */


/*! ___STANDARD INCLUDES_____________________________________________________ */
#include <stdlib.h>

/*! ___PROJECT INCLUDES______________________________________________________ */


/*! ___DEFINITIONS __________________________________________________________ */
#define MAXSTRLEN   64


typedef struct fractionType{
	int numerator, denominator;
} *fraction;

/*! ___FUNCTION PROTOTYPES___________________________________________________ */

int CreateFraction(int numerator, int denominator, fraction *f);
/*! Create a fraction with the specified components. Only the numerator may
 *  be negative and the denominator must be greater than zero. Fractions will
 *  automatically be reduced.
 *  Pre-condition: Denominator > 0 
 *  Post-condition: f' is the desired fraction or undifined if pre-condition 
 *  not met.  */

int DisposeFraction(fraction *f);
/*! Dispose of the fraction f, which becomes undefined (NULL).
 *  Pre-condition: f is defined
 *  Post-condition: f' is undifined (NULL) */

int AddFractions(fraction f1, fraction f2, fraction *result);
/*! Add fractions f1 and f2 to produce result.
 *  Pre-condition: f1 and f2 are defined.
 *  Post-condition: result' = f1 + f2 or NULL on error */

int SubtractFractions(fraction f1, fraction f2, fraction *result);
/*! Subtract fractions f1 and f2 to produce result.
 *  Pre-condition: f1 and f2 are defined.
 *  Post-condition: result' = f1 - f2 or NULL on error */

int MultiplyFractions(fraction f1, fraction f2, fraction *result);
/*! Multiply fractions f1 and f2 to produce result.
 *  Pre-condition: f1 and f2 are defined.
 *  Post-condition: result' = f1 * f2 or NULL on error*/

int DivideFractions(fraction f1, fraction f2, fraction *result);
/*! Divide fraction f1 by f2 to produce result. 
 *  Pre-condition: f1 and f2 are defined and f2 != NULL
 *  Post-condition: result' = f1 / f2 or NULL on error */


#endif 

