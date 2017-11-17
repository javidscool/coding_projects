#include "CodeBook.h"

//including GestureCollection.h here cause of the forward declaration in the .h file
#include "GestureCollection.h"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cfloat>

CodeBook::CodeBook(int nMeans, int minE, GestureCollection *gColl){
	numMeans = nMeans;

	minElements = minE;

	gestColl = gColl;
}

//use this constructor when we LOAD a codebook for classification
CodeBook::CodeBook(GestureCollection *gColl){
	gestColl = gColl;

	//we'll set the other parameters in the Load method
}


void CodeBook::BuildCodeBook(){
	//init. random seed generator
	srand( time(0) );

	//for storing the total number of observation sequences for the gesture we're looking at
	int totalObSeq = -1;

	//variable to store the total number of feature vectors an observation seq. has
	//used for picking a feature vector from an observation sequence
	int totalFeatVects = -1;


	//this vector represents the feature vectors that we've chosen from the specific observation sequences 
	vector<ChosenFeatVect> pickedMeans;
	
	
	//keeping track of how many means we picked so far
	int pickedSoFar = 0;

	//creating variables for the gesture I picked, the observation sequence that I picked 
	//and the feature vector that I picked from that observation sequence
	int pickedGesture = -1;
	int pickedObSeqNum = -1;
	int pickedFeatVectNum = -1;

	
	
	//keep picking means till we have enough
	//we pick randomly selected feature vectors
	while(pickedSoFar < numMeans){
		//generate random number that corresponds to the gesture from which we will choose our observation seqence
		//the number generated is between 0 and the (total number of gestures - 1)
		//pickedGesture = rand() % gestColl->gestCollection.size();


		//We do this because we're only putting in info for the first 4 gestures
		//TODO:  Delete this after we test everything
		pickedGesture = rand() % 2;


		//generate random number that corresponds to the observation sequence to pick from
		//the number generated is between 0 and the (total number of observation sequences in gesture - 1)
		totalObSeq = gestColl->gestCollection[pickedGesture].GetObsSamples().size();
		pickedObSeqNum = rand() % totalObSeq;

		//getting the actual observation sequence that we picked
		ObservationSeq & pickedObSeq = gestColl->gestCollection[pickedGesture].GetObsSamples()[pickedObSeqNum];

		//now that we've picked an observation sequence, pick a feature vector from within that observation sequence
		totalFeatVects = pickedObSeq.GetFeatureVectors().size();
		pickedFeatVectNum = rand() % totalFeatVects;

		//checking to see if the element we picked is already in the list
		ChosenFeatVect meanPicked(pickedGesture, pickedObSeqNum, pickedFeatVectNum);
		//if we already have this ob. seq. as one of our means, try again
		if(IsPickedAlready(meanPicked, pickedMeans)) continue;

		else{
			//put the feature vector we picked into the mean pool
			meanVectors.push_back(pickedObSeq.GetFeatureVectors()[pickedFeatVectNum]);

			//put the picked means into it's vector
			pickedMeans.push_back(meanPicked);

			//mark that we picked a mean
			pickedSoFar++;
		}
	}



	while(true){
		//now that we've picked the means, we gotta group shit to their closest mean
		
		//create our clusters
		vector<FeatureVector> cluster; //temp cluster
		for(int i = 0; i < meanVectors.size(); i++) clusters.push_back(cluster);

		vector<int> clList;

		//going thru the observation sequences of ALL our gestures and creating a cluster list for each gesture
		//for(int gest = 0; gest < gestColl->gestCollection.size(); gest++){
		

		//We do this because we're only putting in info for the first 4 gestures
		//TODO:  Delete this after we test everything
		for(int gest = 0; gest < 2; gest++){

			//cout << "gest: " << gest << endl;

			//getting the observation samples of "currently-looked-at" gesture
			vector<ObservationSeq> & gestureObservations = gestColl->gestCollection[gest].GetObsSamples();

			//so go through all the observation sequences, set their cluster lists, and cluster ALL the feature vectors
			for(int i = 0; i < gestureObservations.size(); i++){

				//cout << "i: " << i << endl; 

				vector<FeatureVector> & featVects = gestureObservations[i].GetFeatureVectors();

				//going through all the feature vectors in THIS observation sequence and placing them in clusters
				for(int j = 0; j < featVects.size(); j++){

					//cout << "j: " << j << endl; 

					//figure out the cluster number to place this feature vector...
					int clusterNum = GetClusterNumber(meanVectors, featVects[j]);

					//cout << "clusterNum: " << clusterNum << endl;

					//...then place it in the appropriate cluster...
					clusters[clusterNum].push_back(featVects[j]);
					//...then add the cluster number to the clList of the observation seq
					clList.push_back(clusterNum);
				}

				//after we get the cluster list, set the observation sequences cluster list to it...
				gestureObservations[i].SetClusterList(clList);

				//...then clear out the cluster list for the next observation sequence
				clList.clear();
			}
		}

		//I'M NOT REMOVING A CLUSTER IF IT HAS LESS THAN minElements
		//SEEMS LIKE TOO MUCH TROUBLE RIGHT NOW...I'LL DO IT IF I NEED TO LATER

		//store the current mean vectors before we set them again for comparison to see if they changed
		prevMeanVectors = meanVectors;
		//now we need to compute a new mean for each cluster
		CalculateNewMeans();


		//now that we've calculated the new means, see if they converge with the previous means
		if(IsMeansConverge()){
			//if they do then we have our codebook
			break;
		}

		else{
			//if they don't then we have to clear out our clusters and try again
			clusters.clear();
		}
	}

}

bool CodeBook::IsPickedAlready(const ChosenFeatVect chosenOne, vector<ChosenFeatVect> pickedMeans){

	//just brute force the find since we don't have that many means that we're picking
	//(we're only picking 32-64 AT THE END, so the passed in vector is shorter)
	for(int i = 0; i < pickedMeans.size(); i++){
		if(pickedMeans[i].IsSame(chosenOne))
			return true;
	}

	return false;
}

int CodeBook::GetClusterNumber(vector<FeatureVector> meanVects, FeatureVector curFeatVect){
	//defining a variable that'll hold our result: the cluster number this feature vector goes in
	int clusterNumber = 0;
	
	double minDist = DBL_MAX;
	double tempDist;

	//going through all the mean feature vectors we picked and seeing which cluster the curFeatVect should go in
	for(int i = 0; i < meanVects.size(); i++){
		tempDist = curFeatVect.Dist(meanVects[i]);

		if(tempDist < minDist){
			minDist = tempDist;
			clusterNumber = i;
		}
	}

	/*
	if(clusterNumber == 0){
		cout << "UH-OH clusterfuck" << endl;
		clusterNumber = 1;
	}
	*/

	return clusterNumber;
}

void CodeBook::CalculateNewMeans(){
	//first look at each cluster
	for(int i = 0; i < clusters.size(); i++){
		//then get the new mean for each cluster
		meanVectors[i] = CalculateOneMean(clusters[i]);
	}
}

FeatureVector CodeBook::CalculateOneMean(vector<FeatureVector> cluster){

	//number of feature vectors in this cluster
	int numFeats = cluster.size();

	//let's start adding up clusters together to avg. them...starting with the first one
	FeatureVector meanCluster = cluster[0];

	//add up all the feature vectors together
	//starting at 1 cause we already took into account the first feat. vect. (above)
	for(int i = 1; i < numFeats; i++){
		//cout << "i: " << i << endl;

		meanCluster = meanCluster + cluster[i];
	}

	//now divide by the total number of feat. vects in this cluster to get our new avg.
	meanCluster = meanCluster / numFeats;

	return meanCluster;
}

bool CodeBook::IsMeansConverge(){
	//the distance that states that the means converge
	//play around with this
	double delta = 0.1;

	for(int i = 0; i < meanVectors.size(); i++){
		//if the two means are too far apart
		if(meanVectors[i].Dist(prevMeanVectors[i]) > delta)
			return false;
	}

	return true;
}


//this function takes an observation sequence and creates a cluster list for it
//DON'T CHANGE THE OBSERVATION SEQUENCE
void CodeBook::CreateClusterListForClassification(ObservationSeq & O){
	vector<int> clList;
	
	//getting the feature vectors of this observation sequence
	vector<FeatureVector> & featVects = O.GetFeatureVectors();

	//going through all the feature vectors in THIS observation sequence and placing them in clusters
	for(int i = 0; i < featVects.size(); i++){

		//figure out the cluster number to place this feature vector...
		int clusterNum = GetClusterNumber(meanVectors, featVects[i]);

		//...then add the cluster number to the clList of the observation seq
		clList.push_back(clusterNum);
	}

	//after we get the cluster list, set the observation sequences cluster list to it...
	O.SetClusterList(clList);
}


//function that saves the codebook
void CodeBook::Save(){
	ofstream fout("codebook.dat");

	//save the two variables
	fout << numMeans << endl;
	fout << minElements << endl;

	//now save all the meanVectors
	for(int i = 0; i < meanVectors.size(); i++){
		meanVectors[i].Save(fout);

		//putting an endl after every save EXCEPT at the last write
		if(i < meanVectors.size() - 1)
			fout << endl;
	}

	//close the connection
	fout.close();
}

//function that loads the codebook
void CodeBook::Load(){
	ifstream fin("codebook.dat");

	//variable to read the ints
	int readVar;

	fin >> readVar;
	numMeans = readVar;

	fin >> readVar;
	minElements = readVar;

	for(int i = 0; i < numMeans; i++){
		FeatureVector myFeatVector;

		myFeatVector.Load(fin);

		meanVectors.push_back(myFeatVector);
	}

	//close the connection
	fin.close();
}