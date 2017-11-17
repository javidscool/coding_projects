#ifndef CODEBOOK_H
#define CODEBOOK_H

#include <iostream>
#include <fstream>

#include "ObservationSeq.h"

using namespace std;

struct ChosenFeatVect{
	int chosenGesture;
	int chosenObSeq;
	int chosenFeatVect;

	ChosenFeatVect(int chGest, int chOb, int chFV){
		chosenGesture = chGest;
		chosenObSeq = chOb;
		chosenFeatVect = chFV;
	}

	bool IsSame(const ChosenFeatVect & fv){
		return (chosenGesture == fv.chosenGesture) && (chosenObSeq == fv.chosenObSeq) && (chosenFeatVect == fv.chosenFeatVect);
	}
};

//forward declaration
class GestureCollection;

class CodeBook{

public:
	CodeBook(int nMeans, int minE, GestureCollection *gColl);

	//use this constructor when we LOAD a codebook for classification
	CodeBook(GestureCollection *gColl);

	//creates our codebook, where we group our feature vectors into clusters
	void BuildCodeBook();


	//this function takes an observation sequence and creates a cluster list for it
	void CreateClusterListForClassification(ObservationSeq & O);

	//function that saves the codebook
	void Save();

	//function that loads the codebook
	void Load();


private:
	int numMeans;
	int minElements;

	GestureCollection *gestColl;

	//our clusters
	vector< vector<FeatureVector> > clusters;

	//mean for each cluster
	vector<FeatureVector> meanVectors;

	vector<FeatureVector> prevMeanVectors;

	bool IsPickedAlready(ChosenFeatVect chosenOne, vector<ChosenFeatVect> pickedMeans);

	int GetClusterNumber(vector<FeatureVector> meanVects, FeatureVector curFeatVect);

	void CalculateNewMeans();

	FeatureVector CalculateOneMean(vector<FeatureVector> cluster);

	bool IsMeansConverge();

};

#endif