#include "ExposureFraction.h"

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>

using namespace std;

//  Diego Casadei, 23 oct 2005

long double ExposureFraction(double ConeHalfAperture, // in radians
			     double cosTheta){

  double inf=M_PI/2-ConeHalfAperture;
  double sup=M_PI/2+ConeHalfAperture;
  
  double theta=acos(cosTheta); // in radians

  long double weight=0.0;
  
  if(inf<theta && theta<sup){
    double cosac=cos(ConeHalfAperture);
    long double tmp=1-cosac*cosac/(1-cosTheta*cosTheta);
    long double radix=0;
    if(tmp>0)radix=sqrt(tmp);
    long double hard=asin(radix);
    weight=hard/M_PI;
  }

  return weight;

}
