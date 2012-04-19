#ifndef STRINGTOFRACTQ1_H
#define STRINGTOFRACTQ1_H
/*! ============================================================================

COMPONENT:                 S T R I N G T O F R A C Q 1. H 
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
#include "fractionsQ1.h"

/*! ___PROJECT INCLUDES______________________________________________________ */



int StringToFraction(char *s, fraction *f);
/*! Attempt to convert the string input to a fraction. The string 
 *  input may either be in mixed form, such as -2 1/3, or 
 *  improrper form, such as 17/3. Whole numbers are accepted and set 
 *  to fraction form. such as 5/1.
 *  Pre-condition: s is a well-formed string
 *  Post-condition: f' is the desired fraction or NULL on error */

int FractionToString(fraction f, char properFraction, char *s);
/*! Convert the fraction f to string format either as a mixed 
 *  number such as 2 1/5, or as an improper fraction such as 11/5,
 *  depending on the parameter properFraction.
 *  Pre-condition: f is defined.
 *  Post-condition: s' is the character representation of the desired
 *  fraction or NULL on error. */ 

float FractionToFloat(fraction f);
/*! Convert fraction f to the corresponding float number.
 *  Pre-condition: f is defined.
 *  Post-condition: return float value corresponding to value of f */


#endif 

