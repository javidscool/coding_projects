#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream> //FILE 
#include <fstream> //FILE
#include "parameters.h"
#include "TravSMan.h"

using namespace std;

//pointer to our traveling salesman object
TravSMan *myTS;

int main(void){

	ChangeType type = BIG;
	
	//-------------------------------------------FILE (Part 1)---------------------------------------------------------------------------
	ofstream myfile;//FILE
	myfile.open("Random Immigrants Results.xls");//FILE

	myfile << "PROGRAM:" << "\t" << "Random Immigrants\n"; // (Warning: Be sure that this is correctly outputing the correct ProgramType)
	myfile << "POPSIZE: " << "\t"<< POP_SIZE << "\n";
	myfile << "NUMCITIES: " << "\t"<< NUM_CITIES << "\n";
	myfile << "MUTATIONRATE: " << "\t"<< MUTATION_RATE << "\n";
	myfile << "CROSSOVERRATE: " << "\t"<< CROSSOVER_RATE << "\n";
	myfile << "CHANGETYPE: "<< "\t" << "SMALL \n"; // (Warning: Be sure that this is correctly outputing the correct ChangeType)
	myfile << "REPLACEMENT RATE: "<< "\t" << REPLACEMENT_RATE << "\n\n"; 
	//-------------------------------------------FILE (Part 1)---------------------------------------------------------------------------

	//seed the random number generator
	srand((unsigned) time(NULL));

	myTS = new TravSMan(MUTATION_RATE, CROSSOVER_RATE, POP_SIZE, NUM_CITIES, type);
	
	double avgBest[NUM_GENS];

	for (int i=0; i < NUM_GENS; i++)
		avgBest[i] = 0;

	//-------------------------FILE (Part 4)-----------------------------
	myfile << "RUN" << "\t" << "GEN"<< "\t" << "BESTFIT\n";
	//-------------------------FILE (Part 4)-----------------------------

	//start the run
	while(myTS->NumRun() < NUM_RUNS){ 
		myTS->Run();
		while(myTS->Started() && myTS->NumGen() < NUM_GENS){
			myTS->Generation();

			if( (myTS->NumGen() % 20) == 0 && type == OSCBIG)
				myTS->tMap->Change(OSCBIG);

			else if( (myTS->NumGen() % 2) == 0 && type == OSCSMALL)
				myTS->tMap->Change(OSCSMALL);

			else if(type == SMALL || type == BIG)
				myTS->tMap->Change(type);

			//-------------------------------FILE (part 5)-----------------------------------------------------
			myfile << myTS->NumRun()<< "\t" << myTS->NumGen() << "\t" << myTS->OutputBest() << "\n";
			//-------------------------------FILE (part 5)-----------------------------------------------------
			avgBest[ myTS->NumGen() - 1 ] += myTS->OutputBest();
		}
	}

	for(int i=0; i < NUM_GENS; i++)
		avgBest[i] = avgBest[i] / NUM_RUNS ; 

	myfile<< "\n\nRuns" << "\t" << "AvgBest\n" << endl;

	for(int i=0; i < NUM_GENS; i++)
		myfile << i << "\t" << avgBest[i]  << endl;

	cout << "End of program.\n";
	myfile.close();//FILE
	system("pause");
	return 0;
}