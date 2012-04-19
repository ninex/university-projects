/*! ============================================================================

COMPONENT:                  F R A C T I O N S Q 1. C
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
#include "fractionsQ1.h"


/*! ___LOCAL DEFINITIONS_____________________________________________________ */


/*! ___LOCAL VARIABLES_______________________________________________________ */


/*! ___MAIN FUNCTION________________________________________________________ */

/*! ***************************************************************************
Parameters:   n1 - first number 
							n2 - second number
Returns:      Returns the greatest common divisor of two numbers >= 1.
Description:  Computes the greatest common divisor of two numbers >= 1
Notes: 
*******************************************************************************/
int GCD(int n1, int n2){
	while ((n1 != 0) & (n2 != 0))
	{
		if (n1>n2)
		{
			n1 = n1 %  n2;
		}else
		{
			n2 = n2 % n1;
		}
	}
	if (n1 == 0)
	{
		return n2;
	}else
	{
		return n1;
	}
}


/*! ***************************************************************************
Parameters:   f - a fraction 
Returns:      A reduced fraction.
Description:  Reduce fraction f so that numerator and denominator have no 
							common factors.
Notes: 
*******************************************************************************/
void Reduce(fraction f){
	if (f != NULL)
	{
		int commonFactor;
		commonFactor = GCD(f->numerator, f->denominator);
		if (commonFactor > 1)
		{
			f->numerator = f->numerator / commonFactor;
			f->denominator = f->denominator / commonFactor;
		}
	}

}


/*! ***************************************************************************
Parameters:   numerator - the numerator of a fraction.
							denominator - the denominator of a fraction.
							f - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description:  Create a fraction with the specified components. Only the 
							numerator may be negative and the denominator must be greater 
							than zero. Values greater than one are specified in improper 
							form, such as 5/2 for 2 1/2. Fractions will automatically be 
							reduced.
Notes: 
*******************************************************************************/
int CreateFraction(int numerator, int denominator, fraction *f){
	if (denominator <= 0)
	{
		return -1;
	}
/*	if (f == NULL)
	{*/

		*f = malloc(sizeof(struct fractionType));
		(*f)->numerator = numerator;
		(*f)->denominator = denominator;
		Reduce(*f);
		return 0;
/*	}else
	{
		return -1;
	}*/
	
}


/*! ***************************************************************************
Parameters:   f - a pointer to a fraction pointer..
Returns:      1 or -1 on error.
Description:  Dispose of the fraction f, which becomes undifined (NULL)
Notes: 
*******************************************************************************/
int DisposeFraction(fraction *f){

	if (f == NULL)
	{
		return 1;
	}else
	{
		free(f);
		return 0;
	}
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description:  Add fractions f1 and f2 to produce result. 
Notes: 
*******************************************************************************/
int AddFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	if ((f1 == NULL) | (f2 == NULL))
	{
		return -1;
	}else
	{
		numerator = (f1->numerator * f2->denominator) + (f2->numerator * f1->denominator);
		denominator = f1->denominator * f2->denominator;
		CreateFraction(numerator, denominator, result);
		return 0;
	}
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Subtract fraction f2 from f1 to produce result. 
Notes: 
*******************************************************************************/
int SubtractFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	if ((f1 == NULL) | (f2 == NULL))
	{
		return -1;
	}else
	{
		numerator = (f1->numerator * f2->denominator) - (f2->numerator * f1->denominator);
		denominator = f1->denominator * f2->denominator;
		CreateFraction(numerator, denominator, result);
		return 0;
	}
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Multiply fractions f1 and f2 to produce result. 
Notes: 
*******************************************************************************/
int MultiplyFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	if ((f1 == NULL) | (f2 == NULL))
	{
		return -1;
	}else
	{
		numerator = f1->numerator * f2->numerator;
		denominator = f1->denominator * f2->denominator;
		CreateFraction(numerator, denominator, result);
		return 0;
	}
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Divide fractions f1 and f2 to produce result. 
Notes: If f2 is initially equal to zero, CreateFraction will return error.
*******************************************************************************/
int DivideFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	if ((f1 == NULL) | (f2 == NULL))
	{
		return -1;
	}else
	{
		numerator = f1->numerator * f2->denominator;
		denominator = f1->denominator * f2->numerator;
		CreateFraction(numerator, denominator, result);
		return 0;
	}
}


