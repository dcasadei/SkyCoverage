#include "ExposureTime.h"

#include<iostream>
#include<cmath>
using namespace std;

#include "ExposureFraction.h"
#include "ChangeRefSys.h"


double ExposureTime(double costheta, double phi,
		    int ctbins, int phibins,
		    int Nrot, double Torbit, double angle,
		    double coneHalfAperture){

  double exposure=0;
  double array[phibins];

  double ctstep=2.0/ctbins;
  double phistep=2*M_PI/phibins;
  double deltaOmega=ctstep*phistep;  // solid angle element

  // find starting bin as function of costheta and phi -> bin
  int firstphibin=static_cast<int>(phi/phistep);
// int ctbin=static_cast<int>((1+costheta)/ctstep);

  int bin=firstphibin;
  double tmpphi=phi;


  if(Nrot==0){ // no histogram rolling
    if(tmpphi<0)tmpphi=2*M_PI+tmpphi; // just not to risk...

    // find ORS theta
    double sintheta=1;
    long double radix=1-costheta*costheta;
    if(radix>0)
      sintheta=sqrtl(radix);
    double ORScosTheta=sintheta*sin(tmpphi)*sin(angle)+costheta*cos(angle);

    exposure=ExposureFraction(coneHalfAperture,ORScosTheta);

    return(exposure*deltaOmega);  // exposure fraction
  }


  for(int i=0; i<=Nrot; ++i){ // loop over histogram rollings
    if(i<phibins){
      if(tmpphi<0)tmpphi=2*M_PI+tmpphi; // just not to risk...

      // find ORS theta
      double sintheta=1;
      long double radix=1-costheta*costheta;
      if(radix>0)
	sintheta=sqrtl(radix);
      double ORScosTheta=sintheta*sin(tmpphi)*sin(angle)+costheta*cos(angle);

      // and find the current bin value (exposure)
      array[bin]=Torbit*ExposureFraction(coneHalfAperture,ORScosTheta);

      // find next value for phi
      tmpphi-=phistep;
    }

    exposure+=array[bin];
    if(bin==0)
      bin=phibins-1;
    else
      --bin;
  } // end loop

  return(exposure*deltaOmega*Nrot);

}


