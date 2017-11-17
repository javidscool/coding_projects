#ifndef CPMATH
#define CPMATH
#include "irrlicht.h"

#define PI 3.14159inline double degreesToRadians(double degrees){	return 2*3.14159*degrees/360;}

inline double radiansToDegrees(double radians){
	return radians*= 57.29578;
}


//finds the quadrant a vector is in
inline int quadrant(vector3df a){
	if(a.X >0 && a.Z>0)
		return 1;
	if(a.X>0 && a.Z<0)
		return 4;
	if(a.X < 0 && a.Z>0)
		return 2;
	if(a.X< 0 && a.Z < 0)
		return 3;
	return 0;
}


//Finds the hypoteneuse
inline double hypo(double opp, double adj){
	double mag;

	//pythagorean theorem
	mag = sqrt((opp*opp)+(adj*adj)); 
	return mag;
}

inline double vectorAngle(irr::core::vector3df t){
	//t.X = fabs(t.X);
	t.Y = 0;
	//t.Z = fabs(t.Z);
	
	double tAngle = acos(fabs(t.X));
	
	switch(quadrant( t.normalize()) ){
		case 1:	break;
		case 2:	tAngle = PI-tAngle;
				break;
		case 3:	tAngle = PI+tAngle;
				break;
		case 4:	tAngle = 2*PI-tAngle;
				break;
		default:	;
	}

	return tAngle;}#endif