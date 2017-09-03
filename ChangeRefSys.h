#ifndef CHANGEREFSYS_H
#define CHANGEREFSYS_H


int ChangeRefSys(double e1, double e2, double e3, 
		 double* costheta, double* phi);

/*
   ChangeRefSys perform an Euler's rotation with angles e1, e2, e3
   respectively (see http://mathworld.wolfram.com/EulerAngles.html) of
   the cos(theta) VS. phi direction.

   The new direction will overwrite old parameters.

   The returned value is 0 if no problem is encountered.  Otherwise:

     1 - bad parameters

   Diego Casadei, 22 oct 2005

*/

#endif
