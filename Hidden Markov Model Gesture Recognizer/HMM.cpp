#include "HMM.h"

//including Gesture.h here cause of the forward declaration in the .h file
#include "Gesture.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cfloat>

using namespace std;

//here is where we'll initilize our HMM,
//using Fig. 8.3 from the htk book
HMM::HMM(const unsigned int n, const unsigned int m){
	N = n;
	M = m;
	
	//now initialize A B and Pi
	initializeABPi();
}

HMM::~HMM(){
}

Eigen::MatrixXd HMM::getA(){
	return A;
}


Eigen::MatrixXd HMM::getB(){
	return B;
}


Eigen::VectorXd HMM::getPi(){
	return Pi;
}


void HMM::initializeABPi(){
	//init. random seed generator
	srand( time(0) );

	//initializing A
	A = Eigen::MatrixXd::Zero(N, N);

	//this seems to work
	//keep in mind this builds a left-right model with no jumps of more than one state allowed
	for (int i = 0; i < N - 1; i++){
		A(i, i) = ( (double)rand() / (double)RAND_MAX );
		A(i, (i + 1) ) = 1 - A(i, i); 
	}

	A( (N - 1), (N - 1) ) = 1.0;

	//cout << A << endl;


	//cout << endl;


	//initializing B
	B = Eigen::MatrixXd::Zero(N, M);

	//this seems to work
	for(int row = 0; row < N; row++){
		for(int col = 0; col < M; col++){
			B(row, col) = (double)1 / (double)M;
		}
	}

	//cout << B << endl;


	//cout << endl;


	//initializing Pi
	Pi = Eigen::VectorXd::Zero(N);

	//we can ONLY start at the first state
	Pi(0) = 1;

	//cout << Pi << endl;

}


//function to initialize our HMM
void HMM::InitializeHMM(Gesture *aGest){

	//setting the gesture here because we're done entering data for it
	attGest = aGest;
	
	const vector<ObservationSeq> & obSeq = attGest->GetObsSamples();

	//this is the difference in max logP's before initialization stops
	//play around with this value to get shit just right
	double delta = 0.01;

	bool isInit = false;

	double logP = 0.0;
	double maxLogP = -(DBL_MAX);
	double prevMaxLogP;

	while(!isInit){
		
		prevMaxLogP = maxLogP;

		//matrix that we use to count the number of transitions between state i and state j
		//it's N x N and is used to update A
		Eigen::MatrixXd countTransitions = Eigen::MatrixXd::Zero(N, N);

		//this matrix counts the number of ob. seq. w/ a specific codebook index in a specific state
		Eigen::MatrixXd BTemp = Eigen::MatrixXd::Zero(N, M);
		//this vector counts the number of observation sequences in each state
		Eigen::VectorXd vectsInState = Eigen::VectorXd::Zero(N);
		//both these vectors are used to update B


		//ok, now we have a vector of observation sequences and we need to run viterbi on all of them
		for(int i = 0; i < obSeq.size(); i++){

			//run viterbi to get the path and the logP that that path produces
			Eigen::VectorXi path = Viterbi(obSeq[i].GetNumSkeletonSamples(), obSeq[i], logP);

			//first update the maxLogP if necessary
			if(logP > maxLogP){
				maxLogP = logP;
			}

			//then get information to update A and B


			//first get info we need to update B
			
			//we need to count the number of observation sequences with specific codebook indexes in specific states
			//IOW we need to know the number of vectors w/ codebook index k in state j

			//to do that we need the cluster list and the path we got from viterbi
			const vector<int> myClustList = obSeq[i].GetClusterList();

			for(int j = 0; j < myClustList.size(); j++){
				//this is counting the number of vectors w/ codebook index myClustList[j] in state path(j)
				BTemp(path(j), myClustList[j])++;

				//this is counting the number of vectors in each state
				vectsInState(path(j))++;
			}



			//now get info we need to update A

			//counting the total number of transitions from state i to state j
			//i'm saying numskeletonsamples - 1 cause i'm counting like this: (state i, state (i + 1) )
			for(int k = 0; k < (obSeq[i].GetNumSkeletonSamples() - 1); k++){
				countTransitions(path(k), path(k + 1))++;
			}

		}

		//now update B and A

		//first update B
		for(int i = 0; i < N; i++){
			//TODO: TEST THIS TO MAKE SURE SHIT DIVIDES PROPERLY
			BTemp.row(i) = BTemp.row(i).array() / vectsInState(i);
		}

		//set B to this BTemp
		B = BTemp;


		//then update A
		for(int i = 0; i < N; i++){

			//TODO: TEST ALL THIS TO MAKE SURE IT WORKS!!!

			//this is sigma A_ik from k = 2 to N
			double summedTrans = 0.0;
			for(int k = 1; k < N; k++){
				summedTrans += countTransitions(i, k);
			}

			//now that we have summedTrans, we can properly update A
			for(int j = 0; j < N; j++){
				A(i, j) = countTransitions(i, j) / summedTrans;
			}

		}

		//now check to see if we can stop initializing
		if(abs(maxLogP - prevMaxLogP) <= delta)
			isInit = true;
	}

}

//the viterbi algorithm finds the single best state sequence for the given observation sequence
Eigen::VectorXi HMM::Viterbi(const unsigned int T, const ObservationSeq & O, double & logP) const{

	//the path variable stores our most likely path
	Eigen::VectorXi path = Eigen::VectorXi::Zero(T);

	//the delta variable stores the best score (highest prob.) along a single path, at time t,
	//which accounts for the first t observations and ends in state Si (all the N's)
	Eigen::MatrixXd delta(T, N);

	//the deltaA variable stores all the delta * (A matrix) multiplications (PER RECURSIVE ITERATION)
	//we need this in order to find the maximum argument in order to get the most likely state
	Eigen::VectorXd deltaA(N);

	//psi stores the most likely state the HMM is going to be in at time t
	//this matrix holds ALL the "most-likely states" that viterbi produces
	Eigen::MatrixXi psi = Eigen::MatrixXi::Zero(T, N);

	//we're going to use induction and recursion to find the best path
	//this means we need to initialize, do induction using recursion, and then terminate

	//we're going to use log initialization since our HMM uses scaling

	//NOTE THAT logBt is log of bi(Ot), in other words, a log of a look up into the B array
	Eigen::MatrixXd logA(N, N), logBt(N, T);
	Eigen::VectorXd logPi(N);
	for(unsigned int i = 0; i < N; i++){
		//turning our Pi's into logPi's
		logPi(i) = log(Pi(i));

		//turning our A's into logA's
		for(unsigned int j = 0; j < N; j++){
			logA(i, j) = log( A(i, j) );
		}

		//turning our B's into logB's
		
		//first we need to get our cluster list from the observation sequence
		//this will allow us to look into the proper place in the B matrix
		const vector<int> myClustList = O.GetClusterList();

		//now use the cluster list (which tells you which cluster each feature vector is in) to look into the B matrix
		//and calculate the logBt matrix
		for(unsigned int t = 0; t < T; t++){
			logBt(i, t) = log( B(i, myClustList[t]) );
		}

		//now initialize delta with what we've got (remember you have a for loop running through all the N's)
		delta(0, i) = logPi(i) + logBt(i, 0);
	}


	
	//now comes the induction through recursion step
	//we need to update delta and keep track of which deltaA was the max for each time interval
	for(unsigned int t = 1; t < T; t++){

		//we need to store the maximum argument of deltaA to place in psi
		//thus variable helps us do that
		std::ptrdiff_t pos;

		for(unsigned int j = 0; j < N; j++){

			//storing the delta * A multiplication for one entire group of i's
			deltaA = delta.row(t-1).transpose().array() + logA.col(j).array();

			//getting the maximum argument for this row of i's...
			deltaA.maxCoeff(&pos);
			//...and putting it in psi
			psi(t, j) = pos;

			//now use the max delta you got and add the logBt to it to get the right delta for the recursion
			delta(t, j) = deltaA(pos) + logBt(j, t);
		}
	}


	//now comes termination, where we take the max number from the last row of delta
	std::ptrdiff_t qstar;
	delta.row(T-1).maxCoeff(&qstar);
	logP = delta.row(T-1)(qstar);
	path(T-1) = qstar;

	//Path backtracking: getting the optimum path
	//each element in psi is basically telling you, "what's the next best node from the best path, at time t-1 of that path, that takes me
	//from time t-1 to time t of that best path and ends in j" 
	for(int t = T - 2; t > -1; t--){
		path(t) = psi(t+1, path(t+1));
	}

	//return this optimum path
	return path;
}



//function to train our HMM
void HMM::TrainHMM(){
	const vector<ObservationSeq> & obSeq = attGest->GetObsSamples();

	//now run Baum-Welch to train this HMM
	BaumWelch(obSeq);
}


//this function executes the Baum-Welch algorithm in order to re-estimate model parameters
//this algorithm uses multiple observation sequences in order so that there is enough data to make reliable estimates
//we keep on updating until (new HMM logP) - (prev HMM logP) < DELTA
//I use P(O|lambda) from bottom of equation 37 of Rabiner paper
#define DELTA 0.001
void HMM::BaumWelch(const vector<ObservationSeq> & ObSeqs){

	//creating vectors of Marices to store all our alpha's (forward variables), beta's (backward variables), etc. 
	//this is to store shit for EACH of the observation sequences we have
	vector<Eigen::MatrixXd> alpha(ObSeqs.size()), beta(ObSeqs.size()), gamma(ObSeqs.size()), numerA(ObSeqs.size()), numerB(ObSeqs.size());
	
	//vectors of eigen vectors to store our denominators for each observation sequence for A and B
	vector<Eigen::VectorXd> denA(ObSeqs.size()), denB(ObSeqs.size());


	//this is temporary storage that we use to calculate all the alphas, betas, gammas, etc.
	//we first store shit in the temp and then add it to the alpha vector
	Eigen::MatrixXd alphaTemp, betaTemp, gammaTemp, numerATemp, nextA(N, N), nextB(N, M);
	
	//vectors that we use to store the scaling factor and other things
	Eigen::VectorXd scale, logHatP(ObSeqs.size()), denomASum(N), denomBSum(N);


	//this vector stores all the probabilities from each observation sequence
	Eigen::VectorXd prob = Eigen::VectorXd::Zero(ObSeqs.size());
	double p = 0.0;
	

	//calculating the top of reestimation of a(i,j), which is the FIXED ERRATA VERSION of equation (111),
	//and other stuff like alpha (forward variable), beta (backward variable), and gamma 
	for(int i = 0; i < ObSeqs.size(); i++){
		logHatP(i) = Forward(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], scale, alphaTemp);

		Backward(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], scale, betaTemp);

		GammaNumerA(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], alphaTemp, betaTemp, scale, gammaTemp, numerATemp, p);
		
		//setting our matrices for EACH observation sequence with the values we got from our help methods
		alpha[i] = alphaTemp;
		beta[i] = betaTemp;
		gamma[i] = gammaTemp;
		numerA[i] = numerATemp;
		prob[i] = p;
	}


	//get the average of all the logP's and compare that avg to a prev. avg. to decide when to stop training
	double logHatPAvg = logHatP.sum() / ObSeqs.size();
	double logHatPAvgPrev;
	

	do
	{
		//setting prev avg. and calculating new one
		logHatPAvgPrev = logHatPAvg;

		//initializing the matrix of our new revised A and B that we're gonna calculate now
		nextA = Eigen::MatrixXd::Zero(N, N);
		nextB = Eigen::MatrixXd::Zero(N, M);

		Eigen::VectorXd nextDenA = Eigen::VectorXd::Zero(N);
		Eigen::VectorXd nextDenB = Eigen::VectorXd::Zero(N);


		for(int i = 0; i < ObSeqs.size(); i++){
			//looking at our next observation sequence
			const ObservationSeq & O = ObSeqs[i];
			unsigned int T = O.GetNumSkeletonSamples();

			//initializing our numerB[i]'s
			numerB[i] = Eigen::MatrixXd::Zero(N, M);


			//we need to get our cluster list from the observation sequence
			//this will allow us to look into the proper place in the B matrix
			const vector<int> myClustList = O.GetClusterList();


			//temp variables to figure out denominators for ONE observation sequence
			denomASum = Eigen::VectorXd::Zero(N), denomBSum = Eigen::VectorXd::Zero(N);

			for(unsigned int j = 0; j < N; j++){

				//this finds the PARTIAL sum of the denominator of A for one of the i's (just the inner sum part)
				//it's finding sum_t = 1 to t - 1 gamma_t(i), we take out the last t from the sum to do this
				double denomA = gamma[i].col(j).sum() - gamma[i](T - 1, j);
				
				//summing the gammas for one observation sequence
				denomASum(j) += denomA;

				//the denominator of B is the same as A (just the inner sum part), for one observation sequence
				denomBSum(j) += denomA;

				//figuring out the numerator for B
				//first we go through all the output clusters
				for(unsigned int k = 0; k < M; k++){

					double numB = 0.0;

					//B(j,myClustList[t + 1])

					//figuring out the partial numerator of B (just the inner sum part)
					for(unsigned int t = 0; t < T - 1; t++){
						//going through all the t's
						//if the output on any of the t's is the same as the cluster number, add in it's gamma 
						if(myClustList[t] == k)
							numB += gamma[i](t, j);
					}

					//figuring out the numerator for the B for JUST this observation sequence (inner sum ONLY)
					numerB[i](j, k) = numB;
				}
			}


			//the next A is just taking into account the numerator so far (division by P_k already taken care of in GammaNumerA)
			nextA += numerA[i];
			
			//the next B is just taking into account the numerator so far
			//this is doing the outside sum of the numerator
			nextB += (numerB[i] / prob[i]);

			denA[i] = denomASum.array() / prob[i];
			denB[i] = denomBSum.array() / prob[i];
		}

		//this is the denominator for our new A and B (adding up all the denominators from EACH observation sequence)
		//I split the setting and addition operations up for clarity's sake
		for(int i = 0; i < ObSeqs.size(); i++){
			nextDenA += denA[i];
			nextDenB += denB[i];
		}

		//with this for loop we're taking our new numerator for our new A and B and dividing by our new denominator for our new A and B
		for(unsigned int i = 0; i < N; i++){
			if( (nextDenA(i) > .00001) || (nextDenA(i) < -.00001) )	// don't divide by 0
				nextA.row(i).array() /= nextDenA(i);

			if( (nextDenB(i) > .00001) || (nextDenB(i) < -.00001) )	// don't divide by 0
				nextB.row(i).array() /= nextDenB(i);
		}

		//now that we've created our new A and B (nextA and nextB), set A and B to it
		A = nextA;
		B = nextB;

		//RE-calculating the top of reestimation of a(i,j), which is the FIXED ERRATA VERSION of equation (111),
		//and other stuff like alpha (forward variable), beta (backward variable), and gamma 
		for(int i = 0; i < ObSeqs.size(); i++){
			logHatP(i) = Forward(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], scale, alphaTemp);

			Backward(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], scale, betaTemp);

			GammaNumerA(ObSeqs[i].GetNumSkeletonSamples(), ObSeqs[i], alphaTemp, betaTemp, scale, gammaTemp, numerATemp, p);
			
			//setting our matrices for EACH observation sequence with the values we got from our help methods
			alpha[i] = alphaTemp;
			beta[i] = betaTemp;
			gamma[i] = gammaTemp;
			numerA[i] = numerATemp;
			prob[i] = p;
		}

		//getting a NEW log average
		logHatPAvg = logHatP.sum() / ObSeqs.size();
	}
	while( (logHatPAvg - logHatPAvgPrev) > DELTA);
	//^^this makes sure the HMM keeps training until the changes get too small (the HMM has stabalized)
	//it has been proven that either the new HMM will be an improvement over the old one or will be the same as the old one, NEVER worse
}



//method to compute P[O | lambda] by summing over the forward variable alpha for all states at final time T
//we store all the alphas in a matrix that's T x N
//returns P[O | lambda], so this basically solves problem 1 of the HMM paper
double HMM::Forward(const unsigned int T, const ObservationSeq & O, Eigen::VectorXd & scale, Eigen::MatrixXd & alpha) const{
	alpha = Eigen::MatrixXd::Zero(T, N);
	scale = Eigen::VectorXd::Zero(T);


	//we need to get our cluster list from the observation sequence
	//this will allow us to look into the proper place in the B matrix
	const vector<int> myClustList = O.GetClusterList();


	//Initialization, use Pi and B to initialize the first row of the forward variable
	alpha.row(0) = Pi.array() * B.col(myClustList[0]).array();

	//storing the scaling coefficient for a sub 1
	//we don't have to multiply all the scaling coefficients now because there's only one, c sub 1 (we'll have to later tho)
	scale(0) = 1/( alpha.row(0).sum() );
	alpha.row(0) *= scale(0);

	//Induction, we do induction through recursion to get the rest of the alphas
	for(int t = 0; t < T-1; t++){
		//using matrix magic to calculate the induction step (a sub (t + 1) = (sum i = 1 to N alpha_t(i) * a_(i, j)) * b_j(O_t+1) )
		//this gives a whole row of alphas (each .row() is a 1 <= j <= N)
		//JAVID: CHECK TO MAKE SURE THIS WORKS!!! (I think the .transpose will fuck things up)
		alpha.row(t+1) = (alpha.row(t) * A).transpose().array() * B.col(myClustList[t + 1]).array();

		//this works because the PREVIOUS alpha includes all the PREVIOUS scaling coefficients
		//therefore all we need to do is multiply in THIS scaling coefficient
		scale(t+1) = 1/(alpha.row(t+1).sum());
		alpha.row(t+1) *= scale(t+1);
	}

	//Termination, we add up all the alpha big T's and that gives us our probability
	//we need to take the log though since we're scaling
	double logP = 0.0;
	//this is doing equation (103) from Rabiner 
	for(unsigned int t = 0; t < T; t++){
		logP += log(scale(t));
	}

	cout << "logP: " << logP << endl;

	//see if neg works and change it back to pos if shit fucks up
	return -(logP);
}


//method to compute the backward variable beta
//all the betas are stored in a matrix that's T x N 
void HMM::Backward(const unsigned int T, const ObservationSeq & O, const Eigen::VectorXd & scale, Eigen::MatrixXd & beta) const{
	//Initialization, set all B's to 1
	beta = Eigen::MatrixXd::Ones(T, N);

	//this is true because D_T = c_T = 1/Sum_alpha_T
	beta.row(T-1) *= scale(T-1);

	//we need to get our cluster list from the observation sequence
	//this will allow us to look into the proper place in the B matrix
	const vector<int> myClustList = O.GetClusterList();

	//Induction: we do induction through recursion to get the rest of the betas
	for(int t = (T - 2); t > -1; t--){
		//using matrix magic to calculate the induction step (b sub (t) = (sum j = 1 to N beta_t+1(j) * a_(i, j) * b_j(O_t+1) ) )
		//this gives a whole row of alphas (each .row() is a 1 <= i <= N)
		//JAVID: CHECK TO MAKE SURE THIS WORKS!!! (I think the .transpose will fuck things up)

		//also remember the PREVIOUS beta includes all the PREVIOUS scaling coefficients
		//therefore all we need to do is multiply in THIS scaling coefficient
		beta.row(t) = (B.col(myClustList[t + 1]).transpose().array() * beta.row(t+1).array()).matrix() * A.transpose() * scale(t);
	}
}


//this function calculates gamma, xi, and the numerator for our reestimation of A
void HMM::GammaNumerA(const unsigned int T, const ObservationSeq & O, const Eigen::MatrixXd & alpha, const Eigen::MatrixXd & beta, const Eigen::VectorXd scale, Eigen::MatrixXd & gamma, Eigen::MatrixXd & numerA, double & p) const{
	//storing shit for gamma, xi and the numerator for gamma
	gamma = Eigen::MatrixXd::Zero(T, N);
	numerA = Eigen::MatrixXd::Zero(N, N);

	Eigen::MatrixXd xi = Eigen::MatrixXd::Zero(N, N);

	//variables to keep track of the greatest P that we'll use to divide the sum of xi's by
	double P;
	double maxP = -(DBL_MAX);

	//we need to get our cluster list from the observation sequence
	//this will allow us to look into the proper place in the B matrix
	const vector<int> myClustList = O.GetClusterList();

	for(int t = 0; t < T; t++){

		gamma.row(t) = alpha.row(t).array() * beta.row(t).array();

		//i'm dividing by the scaling factor according to the erratum I found
		gamma.row(t) /= scale(t);


		//not doing this for (T - 1)
		if(t < T - 1){

			for(unsigned int i = 0; i < N; i++){

				for(unsigned int j = 0; j < N; j++){

					//you're only looking at xi for ONE specific t here
					xi(i, j) = alpha(t,i) * beta(t+1,j) * A(i,j) * B(j,myClustList[t + 1]);

					//getting all the xi(i, j)'s for ONE t and adding them in to the pile
					//this makes it so at the end we'll have added up all the xi's for all the t's (except the last t)
					numerA(i, j) += xi(i, j);
				}

			}

			//now that we've got our xi for this t, sum it up to get the probability and check to see if it's the max prob.
			P = xi.sum();
			if(P > maxP)
				maxP = P;
		}
	}

	//passing the max probability we got back to baum-welch
	p = maxP;

	//here we're dividing the summed up xi's by the best probability we found
	numerA /= maxP;
}


//gives back the logP of the observation sequence
//we're basically just running the forward algorithm and giving back the logP
double HMM::GiveProbabilityOfObsSeq(const ObservationSeq & O){

	double logP;

	Eigen::MatrixXd alphaTemp;

	Eigen::VectorXd scale;

	//getting the logP using the forward alrogithm...
	logP = Forward(O.GetNumSkeletonSamples(), O, scale, alphaTemp);

	//...and returning it
	return logP;
}


//function to save the HMM
void HMM::Save(ofstream & fout){
	fout << N << endl;
	fout << M << endl;

	fout << A << endl;
	fout << B << endl;
	fout << Pi;
	//^^not putting an endl above, we'll take care of this endl in GestureCollection's Save
	//(cause we need to to read shit back properly)
}

//function to load the HMM
void HMM::Load(ifstream & fin){
	//first load N and M
	fin >> N;
	fin >> M;

	double d;
	
	//next load A
	for(int row = 0; row < A.rows(); row++){
		for(int col = 0; col < A.cols(); col++){
			fin >> d;
			A(row, col) = d;
		}
	}

	//next load B
	for(int row = 0; row < B.rows(); row++){
		for(int col = 0; col < B.cols(); col++){
			fin >> d;
			B(row, col) = d;
		}
	}

	//finally load Pi
	for(int row = 0; row < Pi.rows(); row++){
		fin >> d;
		Pi(row) = d;
	}
}