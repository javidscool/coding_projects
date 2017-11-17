#include "util.h"
#include <cmath>


//converts an integer to a string	
string itos(int arg)
{
	ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//converts a float to a string	
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//	clamps the first argument between the second two
void Clamp(double &arg, double min, double max)
{
	if (arg < min)
		arg = min;
	

	else if (arg > max)
		arg = max;
	
}


void Clamp(int &arg, int min, int max)
{
	if (arg < min)
		arg = min;
	

	else if (arg > max)
		arg = max;
	
}