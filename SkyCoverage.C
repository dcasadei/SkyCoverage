#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>

#include<fstream>
#include<string>

using namespace std;

#include "ExposureTime.h"
#include "ChangeRefSys.h"


const int MAXSIZE=1000000; // 1000 x 1000 histogram at most


int main(int argc, char* argv[]){

  char* prog="SkyCoverage";
  char* version="20051216";
  char* title="`Sky coverage of orbital detectors.  Analytical approach'";
  char* journal="arXiv: astro-ph/0511674";

  if(argc<4){
    cerr << "\nThis is `" << prog <<"' version " << version 
	 << ",\nby Diego Casadei (http://cern.ch/casadei/).\n\n"
      "Usage:\n\t"
	 << prog << " ct_div phi_div file [params]\n"
      "where:\n"
      "\tct_div  = number of bins in cos(theta);\n"
      "\tphi_div = number of bins in phi;\n"
      "\tfile    = output file name;\n"
      "\tparams  = (optional) file with orbit parameters.\n"
      "\tIf params is not given, orbit parameters will be asked and saved into\n"
      "\ta file named `orbit-params.dat'.\n\n"
      "For a full explanation, please refer to:\n"
      "  D. Casadei,\n  "
	 << title << ",\n  " << journal << ".\n\n" << prog
	 << " first computes the weights of sky elements in the ORS as follows:\n\n"
      "\tw(theta, phi) = arcsin(sqrt(1 - (cos(alpha)/sin(theta))^2))\n" 
      "\t                * sin(theta) * d theta * d phi / PI\n\n"
      "(orbit with unit period).  Then moves to the PRS, in which it computes\n"
      "the precession and the final exposure of each sky element (in min sr).\n\n";
    return (1);
  }

  int nct=atoi(argv[1]);
  if(nct<1){
    cerr << "Error: ct_div must be a positive integer\n";
    return(1);
  }
  int nphi=atoi(argv[2]);
  if(nphi<1){
    cerr << "Error: phi_div must be a positive integer\n";
    return(1);
  }

  int size=nct*nphi;
  if(size>MAXSIZE){
    cerr << "Error: cannot accept matrices larger than " << MAXSIZE
	 << "values.\n";
    return(1);
  }


  double ctstep=2.0/nct;       // cos(theta) ranges from -1 to 1:
  double phistep=2*M_PI/nphi;  // phi ranges from 0 to 2*PI:

  /// Input orbit parameters

  double alpha=0; 
  double beta=0;
  double torb=0;
  double tprec=0;
  double toper=0;
  double euler1=0;
  double euler2=0;
  double euler3=0;

  FILE *fout=0;
  FILE *forbit=0;

  // opening output file:
  fout=fopen(argv[3], "w");
  if(!fout){
    cerr << "Error: cannot open " << argv[3] << "!\n";
    return(1);
  }

  if(argc==5){ // orbit parameters on file

    char dummy[100];
    ifstream orbit(argv[4]);
    if(!orbit.is_open()){
      cerr << "Error: cannot open " << argv[4] << "!\n";
      return(1);
    }

    cout << "Reading parameters from file " << argv[4] << ":" << endl;

    orbit.getline(dummy,100,':');
    orbit >> alpha;
    cout << "\tcone half aperture = " << alpha << " deg" << endl;
    
    orbit.getline(dummy,100,':');
    orbit >> beta;
    cout << "\torbit inclination = " << beta << " deg" << endl;

    orbit.getline(dummy,100,':');
    orbit >> torb;
    cout << "\torbit period = " << torb << " min" << endl;

    orbit.getline(dummy,100,':');
    orbit >> tprec;
    cout << "\tprecession period = " << tprec << " days" << endl;
    tprec*=(24*60);  // precession period in minutes

    orbit.getline(dummy,100,':');
    orbit >> toper;
    cout << "\tmission duration = " << toper << " days" << endl;
    toper*=(24*60);  // mission duration in minutes

    orbit.getline(dummy,100,':');
    orbit >> euler1;
    euler1 = (euler1/360-int(euler1/360))*360;
    cout << "\tfirst Euler angle = " << euler1 << " deg" << endl;

    orbit.getline(dummy,100,':');
    orbit >> euler2;
    euler2 = (euler2/360-int(euler2/360))*360;
    cout << "\tsecond Euler angle = " << euler2 << " deg" << endl;

    orbit.getline(dummy,100,':');
    orbit >> euler3;
    euler3 = (euler3/360-int(euler3/360))*360;
    cout << "\tthird Euler angle = " << euler3 << " deg" << endl;

    orbit.close();
  }
  else{ // orbit parameters from standard imput
    forbit=fopen("orbit-params.dat", "w");
    if(!forbit){
      cerr << "Error: cannot open orbit-params.dat!\n";
      fclose(forbit);
      return(1);
    }

    cout << "Please, enter the cone half-aperture angle (deg): ";
    cin >> alpha;
    if(alpha<=0 || alpha>=90){
      cerr << "Error: alpha must range from 0 (not included) to 90 deg (not included)\n";
      fclose(forbit);
      return(1);
    }
    fprintf(forbit,"cone half-aperture angle (deg):  %10.4g\n",alpha);

    cout << "Please, enter the orbit inclination (deg):        ";
    cin >> beta;
    if(beta<-90 || beta>90){
      cerr << "Error: beta must range from -90 deg (included) to 90 deg (included)\n";
      fclose(forbit);
      return(1);
    }
    fprintf(forbit,"orbit inclination (deg):         %10.4g\n",beta);

    cout << "Please, enter the orbit period (minutes):         ";
    cin >> torb;
    if (torb<=0) {
      cerr << "Error: the orbit period must be greater than zero.\n";
      fclose(forbit);
      return(1);
    }
    fprintf(forbit,"orbit period (minutes):          %10.4g\n",torb);

    cout << "Precession period (days; 0 means no precession):  ";
    cin >> tprec;
    if (tprec<0) {
      cerr << "Error: the precession period can not be less than zero.\n";
      fclose(forbit);
      return(1);
    }
    fprintf(forbit,"precession period (days):        %10.4g\n",tprec);

    cout << "Please, enter the mission duration (days):        ";
    cin >> toper;
    if (toper<=0) {
      cerr << "Error: the mission duration must be greater than zero.\n";
      fclose(forbit);
      return(1);
    }
    fprintf(forbit,"mission duration (days):         %10.4g\n",toper);

    cout << "\nWe move from the PRS to the GRS with an Euler's rotation (see\n"
      "http://mathworld.wolfram.com/EulerAngles.html).  The following angles are in deg.\n\n"
       "Please, enter the first angle (deg)\n"
      "(rotation about the 3rd axis; 1st axis -> line of nodes): ";
    if(!(cin >> euler1)){
      cerr << "Error: bad value.  The program stops here.\n\n";
      fclose(forbit);
      return(1);
    }
    euler1 = (euler1/360-int(euler1/360))*360;
    fprintf(forbit,"first Euler angle (deg):         %10.4g\n",euler1);

    cout << "Please, enter the second angle (deg)\n"
      "(rotation about the line of nodes; 3rd axis to its final orientation): ";
    if(!(cin >> euler2)){
      cerr << "Error: bad value.  The program stops here.\n\n";
      fclose(forbit);
      return(1);
    }
    euler2 = (euler2/360-int(euler2/360))*360;
    fprintf(forbit,"second Euler angle (deg):        %10.4g\n",euler2);
    
    cout << "Please, enter the third angle (deg)\n"
      "(rotation about the final 3rd axis; line of nodes -> final 1st axis): ";
    if(!(cin >> euler3)){
      cerr << "Error: bad value.  The program stops here.\n\n";
      fclose(forbit);
      return(1);
    }
    euler3 = (euler3/360-int(euler3/360))*360;
    fprintf(forbit,"third Euler angle (deg):         %10.4g\n",euler3);
    cout << endl;
    fclose(forbit);
  }

  // angles in rad:
  alpha=M_PI*alpha/180.0;
  beta=M_PI*beta/180.0;
  euler1=M_PI*euler1/180.0;
  euler2=M_PI*euler2/180.0;
  euler3=M_PI*euler3/180.0;

  tprec*=(24*60);  // precession period in minutes
  if (tprec>0 && tprec<=torb) {
    cerr << "Error: precession cannot be faster than revolution.\n";
    return(1);
  }

  toper*=(24*60);  // mission duration in minutes
  if (toper<=torb) {
    cerr << "Error: mission duration < full orbit.\n";
    return(1);
  }

  double binDuration=tprec/nphi; // min
  int Nro=0;
  if(tprec > 0){
    Nro=int(toper/binDuration+0.5); // no. of rollings

    if(abs(toper-Nro*binDuration)>1e-5){
      cerr << "Warning: the mission duration (" << toper
	   << " min) is not a multiple of the bin duration ("
	   << binDuration << " min).\n" << "I will approximate T_miss = " 
	   << Nro << " * T_bin\n" << endl;
    }
  }

  if (euler1==0 && euler2==0 && euler3==0){
    cerr << "Warning: null rotation angles => PRS.\n\n";
  }

  /// Loop on GRS histogram

  double costheta=ctstep/2.0-1.0; // initial value (center of first bin)
  for (int row=0; row<nct; ++row) { // loop on cos(theta)=-1...+1

    double phi=phistep/2.0; // initial value (center of first bin)
    for (int col=0; col<nphi; ++col){ // loop on phi=0...2*M_PI

      // find solid angle element in PRS
      double prscostheta=costheta;
      double prsphi=phi;
      if(euler1!=0 || euler2!=0 || euler3!=0){ // PRS != GRS
	if(ChangeRefSys(euler1,euler2,euler3,&prscostheta,&prsphi)){
	  cerr << "Error: cannot move from GRS to PRS!\n";
	  return(1);
	}
      }

      // get exposure time for current bin
      double weight=ExposureTime(prscostheta,prsphi,nct,nphi,
				 Nro,torb,beta,alpha);

      fprintf(fout, "%15.7f ", weight);
      phi+=phistep;
    }
    costheta+=ctstep;
    fprintf(fout, "\n");
  }

  fclose(fout);

  return(0);

}

