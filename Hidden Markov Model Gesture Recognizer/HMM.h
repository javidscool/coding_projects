#ifndef HMM_H
#define HMM_H

#include <iosfwd>
#include <iostream>
#include <Eigen/Core>
#include <vector>

#include "ObservationSeq.h"

using namespace std;

//forward declaration
class Gesture;

class HMM{

public:
	HMM(const unsigned int n, const unsigned int m);
	~HMM();

	//VectorXi is a dynamic vector of integers
	//this function executes the Viterbi algorithm to find the best state sequence
	Eigen::VectorXi Viterbi(const unsigned int T, const ObservationSeq & O, double & logP) const;

	//this function executes the Baum-Welch algorithm in order to re-estimate model parameters
	//this algorithm uses multiple observation sequences in order so that there is enough data to make reliable estimates
	void BaumWelch(const vector<ObservationSeq> & ObSeqs);


	Eigen::MatrixXd getA();
	Eigen::MatrixXd getB();
	Eigen::VectorXd getPi();

	//function to initialize our HMM
	void InitializeHMM(Gesture *aGest);

	//function to train our HMM
	void TrainHMM();

	//gives back the logP of the observation sequence
	double GiveProbabilityOfObsSeq(const ObservationSeq & O);

	//function to save the HMM
	void Save(ofstream & fout);

	//function to load the HMM
	void Load(ifstream & fin);


private:

	//method to compute P[O | lambda] by summing over the forward variable alpha for all states at final time T
	//we store all the alphas in a matrix that's T x N
	//returns P[O | lambda], so this basically solves problem 1 of the HMM paper
	double Forward(const unsigned int T, const ObservationSeq & O, Eigen::VectorXd & scale, Eigen::MatrixXd & alpha) const;

	//method to compute the backward variable beta
	//all the betas are stored in a matrix that's T x N 
	void Backward(const unsigned int T, const ObservationSeq & O, const Eigen::VectorXd & scale, Eigen::MatrixXd & beta) const;

	//this function calculates gamma, xi, and the numerator for gamma
	void GammaNumerA(const unsigned int T, const ObservationSeq & O, const Eigen::MatrixXd & alpha, const Eigen::MatrixXd & beta, const Eigen::VectorXd scale, Eigen::MatrixXd & gamma, Eigen::MatrixXd & numerA, double & p) const;



	void initializeABPi(); 


	//number of states
	unsigned int N;

	//number of observations (clusters in the codebook)
	unsigned int M;

	//state transition probability matrix
	//it's N x N
	Eigen::MatrixXd A;

	//observation probability distribution matrix
	//it's N x M
	//it's like saying the prob. that this state (one of the N's) produced an observation from this cluster (one of the M's) 
	Eigen::MatrixXd B;

	//initial state distribution vector
	//it's N x 1
	Eigen::VectorXd Pi;

	//the gesture attached to this HMM
	Gesture *attGest;

};

#endif