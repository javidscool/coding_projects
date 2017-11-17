#include "ObservationSeq.h"
#include <cmath>
#include <assert.h>

//ctor
//here we're getting the skeleton info vector sent to us by the hardware for the gesture we did and are 
//converting each of those SkeletonInfo's into their corresponding feature vectors
ObservationSeq::ObservationSeq(vector<SkeletonInfo> siList){
	assert(siList.size() > 0);

	//set the skeleton list and get the number of samples the list contains
	skeletonInfoList = siList;
	numSkeletonSamples = skeletonInfoList.size();

	//now we need to create our feature vectors for this observation sequence
	for(int i = 0; i < numSkeletonSamples; i++){
		FeatureVector featVect = GetFeatureVector(skeletonInfoList[i]);

		featureVectorList.push_back(featVect);
	}

}

FeatureVector ObservationSeq::GetFeatureVector(SkeletonInfo & skeletonFrame){
	//creating a feature vector to fill in and return
	FeatureVector featVect;

	//getting the unit 3D vector features
	featVect.RS_RE = CalculateRS_RE(skeletonFrame);
	featVect.RE_RH = CalculateRE_RH(skeletonFrame);

	featVect.LS_LE = CalculateLS_LE(skeletonFrame);
	featVect.LE_LH = CalculateLE_LH(skeletonFrame);

	featVect.RH_LH = CalculateRH_LH(skeletonFrame);


	//getting the angle features
	featVect.N_RS_RE = CalculateN_RS_RE(featVect.RS_RE, skeletonFrame);
	featVect.RS_RE_RH = CalculateRS_RE_RH(featVect.RE_RH, skeletonFrame);
	featVect.N_LS_LE = CalculateN_LS_LE(featVect.LS_LE, skeletonFrame);
	featVect.LS_LE_LH = CalculateLS_LE_LH(featVect.LE_LH, skeletonFrame);

	featVect.distRHtoLH = CalculateDistRHtoLH(skeletonFrame);

	featVect.rollLeftHand = skeletonFrame.rollLeftHand;
	featVect.rollRightHand = skeletonFrame.rollRightHand;

	return featVect;

}

//function to get the distance of 2 points
double ObservationSeq::Distance(const JointPoint & pt1, const JointPoint & pt2){
	return sqrt( pow((pt1.x - pt2.x), 2.0) + pow((pt1.y - pt2.y), 2.0) + pow((pt1.z - pt2.z), 2.0) );
}

//function to get the dot product
double ObservationSeq::DotProduct(const JointPoint & pt1, const JointPoint & pt2){
	return ( (pt1.x * pt2.x) + (pt1.y * pt2.y) + (pt1.z * pt2.z) );
}

//function to get the RS->RE feature
JointPoint ObservationSeq::CalculateRS_RE(SkeletonInfo & skeletonFrame){
	JointPoint rs_re = skeletonFrame.rightElbowPos - skeletonFrame.rightShoulPos;

	double dist = Distance(skeletonFrame.rightElbowPos, skeletonFrame.rightShoulPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//returning the unit vector
	return rs_re / dist;
}


//function to get the RE->RH feature
JointPoint ObservationSeq::CalculateRE_RH(SkeletonInfo & skeletonFrame){
	JointPoint re_rh = skeletonFrame.rightHandPos - skeletonFrame.rightElbowPos;

	double dist = Distance(skeletonFrame.rightHandPos, skeletonFrame.rightElbowPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//returning the unit vector
	return re_rh / dist;
}

//function to get the LS->LE feature
JointPoint ObservationSeq::CalculateLS_LE(SkeletonInfo & skeletonFrame){
	JointPoint ls_le = skeletonFrame.leftElbowPos - skeletonFrame.leftShoulPos;

	double dist = Distance(skeletonFrame.leftElbowPos, skeletonFrame.leftShoulPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//returning the unit vector
	return ls_le / dist;
}

//function to get the LE->LH feature
JointPoint ObservationSeq::CalculateLE_LH(SkeletonInfo & skeletonFrame){
	JointPoint le_lh = skeletonFrame.leftHandPos - skeletonFrame.leftElbowPos;

	double dist = Distance(skeletonFrame.leftHandPos, skeletonFrame.leftElbowPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//returning the unit vector
	return le_lh / dist;
}
	
//function to get the RH->LH feature
JointPoint ObservationSeq::CalculateRH_LH(SkeletonInfo & skeletonFrame){
	JointPoint rh_lh = skeletonFrame.leftHandPos - skeletonFrame.rightHandPos;

	double dist = Distance(skeletonFrame.leftHandPos, skeletonFrame.rightHandPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//returning the unit vector
	return rh_lh / dist;
}

//function to calculate the angle N-RS-RE feature
double ObservationSeq::CalculateN_RS_RE(JointPoint & RSRE, SkeletonInfo & skeletonFrame){
	//RSRE is already normalized, but we've gotta normalize RS->N
	JointPoint rs_n = skeletonFrame.neckPos - skeletonFrame.rightShoulPos;

	double dist = Distance(skeletonFrame.neckPos, skeletonFrame.rightShoulPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//normalizing RS->N
	rs_n = rs_n / dist;

	//now we can use dot product and acos to determine the angle between the two vectors
	double dotRSN_RSRE = DotProduct(RSRE, rs_n);

	//return the angle, which is the acos of the dot product
	double result = acos(dotRSN_RSRE); 
	
	return result;
}


//function to calculate the angle RS-RE-RH feature
double ObservationSeq::CalculateRS_RE_RH(JointPoint & RERH, SkeletonInfo & skeletonFrame){
	//RERH is already normalized, but we've gotta normalize RE->RS
	JointPoint re_rs = skeletonFrame.rightShoulPos - skeletonFrame.rightElbowPos;

	double dist = Distance(skeletonFrame.rightShoulPos, skeletonFrame.rightElbowPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//normalizing RE->RS
	re_rs = re_rs / dist;

	//now we can use dot product and acos to determine the angle between the two vectors
	double dotRERS_RERH = DotProduct(re_rs, RERH);

	//return the angle, which is the acos of the dot product
	double result = acos(dotRERS_RERH);

	return result;
}

//function to calculate the angle N-LS-LE feature
double ObservationSeq::CalculateN_LS_LE(JointPoint & LSLE, SkeletonInfo & skeletonFrame){
	//LSLE is already normalized, but we've gotta normalize LS->N
	JointPoint ls_n = skeletonFrame.neckPos - skeletonFrame.leftShoulPos;

	double dist = Distance(skeletonFrame.neckPos, skeletonFrame.leftShoulPos);



	if(dist == 0){
		cout << "UH-OH" << endl;
	}



	//normalizing LS->N
	ls_n = ls_n / dist;

	//now we can use dot product and acos to determine the angle between the two vectors
	double dotLSN_LSLE = DotProduct(LSLE, ls_n);

	//return the angle, which is the acos of the dot product
	double result = acos(dotLSN_LSLE);

	return result;
}

//function to calculate the angle LS-LE-LH feature
double ObservationSeq::CalculateLS_LE_LH(JointPoint & LELH, SkeletonInfo & skeletonFrame){
	//LELH is already normalized, but we've gotta normalize LE->LS
	JointPoint le_ls = skeletonFrame.leftShoulPos - skeletonFrame.leftElbowPos;

	double dist = Distance(skeletonFrame.leftShoulPos, skeletonFrame.leftElbowPos);

	if(dist == 0){
		cout << "UH-OH" << endl;
	}

	//normalizing LE->LS
	le_ls = le_ls / dist;

	//now we can use dot product and acos to determine the angle between the two vectors
	double dotLELS_LELH = DotProduct(le_ls, LELH);

	//return the angle, which is the acos of the dot product
	double result = acos(dotLELS_LELH);

	return result;
}


double ObservationSeq::CalculateDistRHtoLH(const SkeletonInfo & skeletonFrame){
	return Distance(skeletonFrame.rightHandPos, skeletonFrame.leftHandPos);
}

int ObservationSeq::GetNumSkeletonSamples() const{
	return numSkeletonSamples;
}

vector<FeatureVector> & ObservationSeq::GetFeatureVectors(){
	return featureVectorList;
}

void ObservationSeq::SetClusterList(vector<int> clList){
	//the clusterList HAS to be the same as the featureVectorList
	assert(clList.size() == featureVectorList.size());

	clusterList = clList;
}

const vector<int> ObservationSeq::GetClusterList() const{
	return clusterList;
}


	
