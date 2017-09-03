#include "ChangeRefSys.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
using namespace std;

//  Diego Casadei, 22 oct 2005


int ChangeRefSys(double e1, double e2, double e3, // angles in rad
		 double* costheta, double* phi){


  if(*costheta < -1 || *costheta > 1 || *phi < 0 || *phi > 2*M_PI){
    cerr << "ChangeRefSys-error: bad direction!\n";
    return(1);
  }


  /// Transformation matrix (see http://mathworld.wolfram.com/EulerAngles.html)
  double coef[3][3]={
    cos(e3)*cos(e1)-cos(e2)*sin(e1)*sin(e3),  // a11
    cos(e3)*sin(e1)+cos(e2)*cos(e1)*sin(e3),  // a12
    sin(e3)*sin(e2),                          // a13
    -sin(e3)*cos(e1)-cos(e2)*sin(e1)*cos(e3), // a21
    -sin(e3)*sin(e1)+cos(e2)*cos(e1)*cos(e3), // a22
    cos(e3)*sin(e2),                          // a23
    sin(e2)*sin(e1),                          // a31
    -sin(e2)*cos(e1),                         // a32
    cos(e2)                                   // a33
  };


  /// Find old direction
  double sintheta=1;
  long double radix=1-(*costheta)*(*costheta);
  if(radix>0)
    sintheta=sqrtl(radix);
  
  double r[3]={sintheta*cos(*phi),
	       sintheta*sin(*phi),
	       *costheta}; // direction in old ref.sys.

#ifdef DEBUG
  cout << "ChangeRefSys - old dir = {" << *costheta << ", " << *phi << "} = ("
       << r[0] << ", " << r[1] << ", " << r[2] << ")   ";
#endif

  // trasform (x, y, z) -> (X, Y, Z)
  double R[3]; // direction in new ref.sys.
  for(int k=0; k<3; ++k){ // for each new component
    R[k]=0;
    for(int c=0; c<3; ++c)
      R[k]+=coef[k][c]*r[c];
  }

  // find new direction (cosTheta, Phi)
  double newct=R[2];
  double newphi=0;
  if(R[0]!=0)newphi=M_PI+atan2(R[1],R[0]);

  *costheta=newct;
  *phi=newphi;

#ifdef DEBUG
  cout << "new dir = {" << *costheta << ", " << *phi << "} = ("
       << R[0] << ", " << R[1] << ", " << R[2] << ")" << endl;
#endif

  return(0);
}
