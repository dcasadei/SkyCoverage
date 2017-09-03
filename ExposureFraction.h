#ifndef EXPOSUREFRACTION_H
#define EXPOSUREFRACTION_H



long double ExposureFraction(double ConeHalfAperture, // in radians
			     double cosTheta);

  /*  ExposureFraction returns the fraction of the orbit period during
      which the given ring (defined by cosTheta) is inside the
      detector field of view (defined by ConeHalfAperture).

      The returned value should be multiplied by the orbit period to
      get the time spent inside the field of view, and for the solid
      angle element to obtain the exposure.

      Diego Casadei, 14 oct 2005
   */



#endif
