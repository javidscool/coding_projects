#include "TravSMan.h"


//checks if a given integer is already contained in a vector of integers
bool TestNumber(const vector<int> &vec, const int &number)
{
	for (int i=0; i<vec.size(); ++i){
		if (vec[i] == number)
			return true;
	}

	return false;
}

//creates a random tour of the cities when the run starts
vector<int> TSMPath::GrabPermutation(int &numCities){
	
	//create a new vector
	vector<int> vecPerm;
	
	for (int i = 0; i < numCities; i++)
	{
		//we use numCities-1 because we want ints numbered from zero
		int NextPossibleNumber = RandInt(0, numCities-1);

		while(TestNumber(vecPerm, NextPossibleNumber))
		{
			//give me another random number if the previous one is already in the list
			NextPossibleNumber = RandInt(0, numCities-1);
		}

		vecPerm.push_back(NextPossibleNumber);
	}

	return vecPerm;
}


//-----------------------CalculatePopulationsFitness--------------------------
//
//	calculates the fitness of each member of the population, updates the
//	fittest, the worst, keeps a sum of the total fittness scores and the
//	average fitness score of the population
//-----------------------------------------------------------------------------
void TravSMan::CalculatePopulationFitness(){

	for(int i = 0; i < popSize; i++){
		//get the tour length for this member
		double tourLength = tMap->GetTourLength(population[i].vecCities);

		//get tour length of all members
		population[i].fitScore = tourLength;

		//keeping track of the shortest route found per gen
		if(tourLength < shortestRoute){
			shortestRoute = tourLength;
		}

		//keeping track of the worst route found per gen
		if(tourLength > longestRoute){
			longestRoute = tourLength;
		}
	}//move on to next population member

/*
	//normalize fitness scores so that high tour lengths have low fitness scores
	for(int i = 0; i < popSize; i++){
		population[i].fitScore = longestRoute - population[i].fitScore;
	}
*/
	//calculate statistics
	CalculateBestWorstAvTot();
}

//-----------------------CalculateBestWorstAvTot-----------------------	
//
//	calculates the fittest and weakest member and the average/total 
//	fitness scores
//---------------------------------------------------------------------
void TravSMan::CalculateBestWorstAvTot(){
	totalFitness = 0.0;

	double max = -99999999;
	double min = 99999999;

	for(int i = 0; i < popSize; i++){
		//checking for worst fittest and updating if necessary
		if(population[i].fitScore > max){
			max = population[i].fitScore;

			//fittestTMan = i;

			worstFitness = max;

			//bestFitness = max;
		}

		//checking for best fittest and updating if necessary
		if(population[i].fitScore < min){

			min = population[i].fitScore;

			fittestTMan = i;

			bestFitness = min;

			//worstFitness = min;
		}

		totalFitness += population[i].fitScore;
	}//move on to the next population member

	averageFitness = totalFitness / popSize;

}


//---------------------------- TournamentSelection -----------------
//
//  performs standard tournament selection given a number of members to
//  sample from each try.
//------------------------------------------------------------------------

TSMPath & TravSMan::TournamentSelection(int NumToTry){
	double bestFit = 9999999999.0;
	
	int chosenOne = 0;

	//select NumToTry from the population at random testing against the best found so far
	for(int i = 0; i < NumToTry; i++){
		int curPerson = RandInt(0, popSize - 1);

		if(population[curPerson].fitScore < bestFit){
			chosenOne = curPerson;
			bestFit = population[curPerson].fitScore;
		}
	}

	//return the champion
	return population[chosenOne];
}


//--------------------------MutateIM------------------------------
//
//	Chooses a random gene and inserts displaced back into the
//	chromosome
//-----------------------------------------------------------------
void TravSMan::InsertionMutation(std::vector<int> &chromo, double vMutationRate){
	
	//checking if we even do mutation
	if(RandFloat() > vMutationRate) return;

	//create an iterator
	vector<int>::iterator curPos;

	//choose a gene to move
	curPos = chromo.begin() + RandInt(0, chromo.size() - 1);

	int geneValue = *curPos;

	//remove gene from chromosome
	chromo.erase(curPos);

	//move the iterator to a random insertion location
	curPos = chromo.begin() + RandInt(0, chromo.size() - 1);

	chromo.insert(curPos, geneValue);
}

//-------------------------CrossoverPMX---------------------------------
//
// crossover operator based on 'partially matched crossover' as 
// defined in the text
//-------------------------------------------------------------------
void TravSMan::PMX(const vector<int> &parent1, const vector<int> &parent2, vector<int> &child1, vector<int> &child2){
	child1 = parent1;
	child2 = parent2;
	
	//just return if we don't hit the crossover rate or if the chromosomes are the same
	if ( (RandFloat() > crossoverRate) || (parent1 == parent2)) return;
	

	//first we choose a section of the chromosome
	int beg = RandInt(0, parent1.size()-2);
	
	int end = beg;
	
	//find an end
	while (end <= beg)
	{
		end = RandInt(0, parent1.size()-1);
	}

	//now we iterate through the matched pairs of genes from beg
	//to end swapping the places in each child
	vector<int>::iterator posGene1, posGene2;

	for(int pos = beg; pos < end+1; pos++)
	{
		//these are the genes we want to swap
		int gene1 = parent1[pos];
		int gene2 = parent2[pos];

		if (gene1 != gene2)
		{
			//find and swap them in baby1
			posGene1 = find(child1.begin(), child1.end(), gene1);
			posGene2 = find(child1.begin(), child1.end(), gene2);

			swap(*posGene1, *posGene2);

			//and in baby2
			posGene1 = find(child2.begin(), child2.end(), gene1);
			posGene2 = find(child2.begin(), child2.end(), gene2);
			
			swap(*posGene1, *posGene2);
		}
		
	}//next pair of values to be swapped
}	


//-----------------------CreateStartingPopulation()------------------------
//
//	clears any existing population, fills a vector with a random population
//	of genomes and resets appropriate member variables
//-------------------------------------------------------------------------
void TravSMan::CreateStartingPop(){

	//clear the current population vector
	population.clear();

	//create a new population with random members
	for(int i = 0; i < popSize; i++){
		population.push_back(TSMPath(numCities));
	}

	//initialize rest of variables
	numGen = 0;
	shortestRoute = 9999999;
	bestFitness = 0;
	worstFitness = 9999999;
	averageFitness = 0;
	fittestTMan= 0;
    started = false;
}


//-------------------------Reset()------------------------------
//	resets all the relevant variables for a new generation
//--------------------------------------------------------------
void TravSMan::Reset(){

	//make the shortest route some excessively large distance
	shortestRoute = 999999999;
	longestRoute = 0;
	totalFitness = 0;
	bestFitness = 0;
	worstFitness = 9999999;
	averageFitness = 0;
	sorted = false;
}

//------------------------Generation-------------------------------
//	creates a new population of genomes using the selection,
//	mutation and crossover operators
//--------------------------------------------------------------
void TravSMan::Generation(){

	//first we reset variables and calculate the fitness of each genome
	Reset();
	
	CalculatePopulationFitness();

	sort(population.begin(), population.end());

	//we prepare mutation rate for change
	//If Hypermutation is necessary, change the mutation rate to Hypermutation Rate
	//>> else, keep base mutation rate
	double variableMutationRate = AdaptiveMechanism(bestFitness);

	if(variableMutationRate == 0.4)
		cout << "We're doing gradient hypermutation.\n";

	double tempMR = (variableMutationRate + mutationRate)/2;

	//label low mid high mutation lable
	for(int i = 0; i< (popSize*.2); i++)
		population[i].mutRate = mutationRate;
	for(int i = (popSize*.2); i< (popSize*.8); i++)
		population[i].mutRate = tempMR;
	for(int i = (popSize*.8); i < popSize; i++)
		population[i].mutRate = variableMutationRate;

	//if we have found a solution exit
	if ((shortestRoute <= tMap->BestPossibleRoute()))
	{
		started = false;
		
		//increment generation counter
		numGen++;

		cout << "<<<<<Best Found>>>>>" << endl;
		cout << "Best fitness of run " << numRun << " generation " << numGen << " : " << bestFitness << endl;
		cout << "Worst fitness of run " << numRun << " generation " << numGen << " : " << worstFitness << endl;
		cout << endl;

		arrayCount = 0;

		return;
	}	
	
	//create a temporary vector for the new population
	vector<TSMPath> vecNewPop;

	//now create the population
	while (vecNewPop.size() != popSize){
		
		//create the parents
		TSMPath parent1, parent2;
		
		parent1 = TournamentSelection(NUM_TO_COMPARE);
	    parent2 = TournamentSelection(NUM_TO_COMPARE);

		//create 2 children
		TSMPath child1, child2;
		  
		//perform crossover
		PMX(parent1.vecCities,
            parent2.vecCities,
            child1.vecCities,
            child2.vecCities);
		
		//perform mutation

		InsertionMutation(child1.vecCities, parent1.mutRate);
		InsertionMutation(child2.vecCities, parent2.mutRate);

		//add children to new population
		vecNewPop.push_back(child1);
		vecNewPop.push_back(child2);
	}
	
	//copy into next generation
	population = vecNewPop;

	//increment generation counter
	numGen++;

	cout << "Best fitness of run " << numRun << " generation " << numGen << " : " << bestFitness << endl;
	cout << "Worst fitness of run " << numRun << " generation " << numGen << " : " << worstFitness << endl;
	cout << "Current Mutation Rate: " << variableMutationRate << endl;
	cout << endl;
}

//-----------------------------------Run-----------------------------------
//	This is the function that initializes a new population and sets the
//  ga running 
//------------------------------------------------------------------------
void TravSMan::Run(){
	numRun++;

	tMap->Reset();

	if(type == OSCBIG || type == OSCSMALL){
		tMap->GetOscCoords();
	}

	//the first thing we have to do is create a random population
	CreateStartingPop();

	//set variable that says run is going
	started = true;
}

//-------FILE (part 3)-------------------
double TravSMan::OutputBest(){
	return bestFitness;
}
//-------FILE (part 3)-------------------

double TravSMan::AdaptiveMechanism(double currentBestFitness){
	double sumBestFitness = 0.0;
	double avgBestFitness;

	//if the BestFitnessArray is not filled
	if(arrayCount < 5){
		BestFitnessArray[arrayCount] = currentBestFitness;
		arrayCount++;

		for(int i = 0; i < 5; i++)
			sumBestFitness += BestFitnessArray[i];

		avgBestFitness = sumBestFitness / 5;

		previousBestFitness = avgBestFitness;

		return mutationRate;
	}

	//else if BestFitnessarray is full
	else{
		//move each value back, deleting first value and adding last value with currentBestFitness
		for(int i = 0; i <4; i++ ){
			BestFitnessArray[i] = BestFitnessArray[i+1];
		}
		BestFitnessArray[4] = currentBestFitness;

		for(int i = 0; i < 5; i++)
			sumBestFitness += BestFitnessArray[i];

		avgBestFitness = sumBestFitness / 5;

		if( avgBestFitness > previousBestFitness || avgBestFitness ==  previousBestFitness ){
			//cout << "Prevoius Average Best: " << previousBestFitness << endl;
			//cout << "Current Average Best: " << avgBestFitness << endl;
			previousBestFitness = avgBestFitness;

			return hyperMutationRate;
		}
		else{
			//cout << "Previous Average Best: " << previousBestFitness << endl;
			//cout << "Current Average Best: " << avgBestFitness << endl;
			previousBestFitness = avgBestFitness;
			return mutationRate;
		}
	}
}

		
