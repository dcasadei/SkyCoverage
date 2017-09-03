#ifndef EXPOSURETIME_H
#define EXPOSURETIME_H


double ExposureTime(double costheta, double phi,
		    int ctbins, int phibins,
		    int Nrot, double Torbit, double angle,
		    double coneHalfAperture);

/*
  ExposureTime returns the exposure time of the PRS sky element
  containing the direction (costheta, phi) viewed by a detector with a
  conical field of view with half-aperture coneHalfAperture, given the
  orbit revolution period Torbit, the number Nrot of phi-columns
  rotations, and the number of histogram bins in cos(theta) and phi
  (in rad).  The orbit inclination in the PRS is angle.

  Diego Casadei, 19 oct 2005

 */

#endif
