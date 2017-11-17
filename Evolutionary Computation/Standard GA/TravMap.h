#ifndef TRAV_MAP
#define TRAV_MAP

#include <vector>
#include <cmath>

#include "util.h"
#include "parameters.h"

using namespace std;

const double pi = 3.1415926535897;

//---------------------------------------------------------
//	define a simple struct to hold the coordinates of each
//	city
//---------------------------------------------------------
struct Coord{
	int id;

	float x, y;
	
	Coord() : id(-1), x(-1), y(-1){}
	Coord(int i, float a, float b) : id(i), x(a),y(b){}
};


//our actual TravMap class
class TravMap{
private:
	//number of cities in the problem
	int numCities;

	ChangeType type;

	Coord startOsc, endOsc;

	//rectangle dimensions that city circle is in
	int mapWidth;
	int	mapHeight;

	//holds the length of the solution, if one is calculable
	double bestPossibleRoute;

	//calculate the coordinates of each city (positioned in a circle)
	void CreateCircularCities();

	//calculates the distance between two cities
	double CalculateA_To_B(const Coord &city1, const Coord &city2);

	void CalculateBestPossibleRoute();

	//changing functions
	double CalculateRealDist(int num1, int num2);

	void SmallChange();
		
	void BigChange();
		
	void OscillationBig();
	
	void OscillationSmall();


public:
	//the city coordinates
	vector<Coord> vecCityCoords;

	void Change(ChangeType type);

	void Reset();

	TravMap(int w, int h, int nc, ChangeType t) : mapWidth(w), mapHeight(h), numCities(nc), type(t){

		CreateCircularCities();

		CalculateBestPossibleRoute();
	}

	//calculates the tour length
	double GetTourLength(const vector<int> &route);

	//accessor methods 
	inline double BestPossibleRoute() const { return bestPossibleRoute; }

	//get oscillation cities
	void GetOscCoords();

};

#endif