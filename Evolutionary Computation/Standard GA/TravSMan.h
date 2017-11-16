#ifndef TRAV_S_MAN
#define TRAV_S_MAN

#include <iostream>
#include <vector>
#include <algorithm>

#include "parameters.h"
#include "TravMap.h"
#include "util.h"

using namespace std;

//used in GrabPermutation and in PBX crossover. Tests if an 
//integer is already contained in a vector of integers
bool TestNumber(const vector<int> &vec, const int &number);

//this is the path that the salesman takes, 100 possible paths make up our population
struct TSMPath{
	
	//the city tour
	vector<int> vecCities;
	
	//its fitness
	double fitScore;


	//ctor
	TSMPath() : fitScore(0){}
	
	TSMPath(int numCities) : fitScore(0)
	{
		vecCities = GrabPermutation(numCities);
	}

	//creates a random tour of the cities when the run starts
	vector<int>	GrabPermutation(int &numCities);

	//overload '<' used for sorting
	friend bool operator<(const TSMPath& lhs, const TSMPath& rhs)
	{
		return (lhs.fitScore < rhs.fitScore);
	}	
};


//our actual TravSMan class
class TravSMan{
private:
	vector<TSMPath> population;

	ChangeType type;

	double mutationRate;

	double crossoverRate;

	//total fitness of the entire population
	double totalFitness;

	//the shortest tour found so far
	double shortestRoute;

	//the worst tour found so far
	double longestRoute;

	//the best fitness score of the most recent generation
	double bestFitness;

	//the worst fitness score of the most recent generation
	double worstFitness;

	//the average fitness of the most recent generation
	double averageFitness;

	//number of members in the population
	int popSize;

	//length of the chromosome (number of cities)
	int numCities;

	//the fittest member of the most recent generation
	int fittestTMan;

	//keeps track of which generation we're in
	int numGen;

	//keeps track of which run we're in
	int numRun;

	//keeps track if the current run is in progress
	bool started;

	//keeps track of whether the population has been sorted by fitness
	bool sorted;

	//insertion mutation function
	void InsertionMutation(vector<int> &chromo);

	//partially mapped crossover function
	void PMX(const vector<int> &parent1, 
			 const vector<int> &parent2,
			 vector<int> &child1,
			 vector<int> &child2);

	//tournament selection function
	TSMPath & TournamentSelection(int NumToTry);


	//fitness function methods
	void CalculatePopulationFitness();

	void CalculateBestWorstAvTot();
	
	//other methods
	void Reset();

	void CreateStartingPop();

public:
	//instance of the map class
	TravMap *tMap;

	//ctor
	TravSMan(double mutation_rate,
			 double crossover_rate,
			 int pop_size,
			 int num_cities,
			 ChangeType t
			 ) : mutationRate(mutation_rate),
			     crossoverRate(crossover_rate),
				 popSize(pop_size),
				 type(t),
				 fittestTMan(0),
				 bestFitness(-999999),
				 worstFitness(99999999),
				 averageFitness(0),
				 numGen(0),
				 numRun(0),
				 shortestRoute(999999999),
				 longestRoute(0),
				 numCities(num_cities),
				 started(false),
				 sorted(false){
					 //set up the map
					 tMap = new TravMap(MAP_WIDTH, MAP_HEIGHT, num_cities, t);
					
					 //then create your starting population
					 CreateStartingPop();
	}

	~TravSMan(){
		delete tMap;
	}

	void Generation();

	void Run();

	//-----FILE (Part 2)-------
	double OutputBest();
	//-----FILE (part 2)-------

	inline void Stop() { started = false; }
	inline bool Started() const { return started; }
	inline int NumRun() const { return numRun; }
	inline int NumGen() const { return numGen; }

};

#endif
