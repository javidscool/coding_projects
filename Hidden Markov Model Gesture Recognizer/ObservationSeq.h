#ifndef OBSERVATION_SEQ
#define OBSERVATION_SEQ

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

//struct that represents the position of a joint in world space
struct JointPoint{
	double x;
	double y;
	double z;

	//ctors
	JointPoint(double xCoor, double yCoor, double zCoor){
		x = xCoor;
		y = yCoor;
		z = zCoor;
	}

	JointPoint(){
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	JointPoint operator-(const JointPoint &jp) const{
		return JointPoint(x - jp.x, y - jp.y, z - jp.z);
	}

	JointPoint operator+(const JointPoint &jp) const{
		return JointPoint(x + jp.x, y + jp.y, z + jp.z);
	}

	JointPoint operator*(double mult) const{
		return JointPoint(x * mult, y * mult, z * mult);
	}

	JointPoint operator/(double div) const{
		return JointPoint(x / div, y / div, z / div);
	}

	bool operator==(const JointPoint &jp) const{
		return (x == jp.x) && (y == jp.y) && (z == jp.z);
	}

	JointPoint & operator=(const JointPoint &jp){
		x = jp.x;
		y = jp.y;
		z = jp.z;

		return *this;
	}

	//the distance between two JointPoints (how similar they are)
	double Dist(const JointPoint &jp) const{
		return fabs(x - jp.x) + fabs(y - jp.y) + fabs(z - jp.z);
	}

	//function to save the JointPoint
	//saved as: x y z
	void Save(ofstream & fout){
		fout << x << " " << y << " " << z << endl;
	}

	//function to load the JointPoint
	void Load(ifstream & fin){

		fin >> x;
		fin >> y;
		fin >> z;
	}
};

//struct that represents all the information that i'll get from the skeleton
struct SkeletonInfo{
	
	//the position of the neck
	JointPoint neckPos;
	
	//the positions of the right side
	JointPoint rightShoulPos;
	JointPoint rightElbowPos;
	JointPoint rightHandPos;

	//the positions of the left side
	JointPoint leftShoulPos;
	JointPoint leftElbowPos;
	JointPoint leftHandPos;

	//roll angle of left and right hands in radians
	double rollLeftHand;
	double rollRightHand;

	SkeletonInfo(){}

	SkeletonInfo(JointPoint nPos, JointPoint rsPos, JointPoint rePos, JointPoint rhPos, JointPoint lsPos, JointPoint lePos, JointPoint lhPos, double rollLeft, double rollRight){
		neckPos = nPos;

		rightShoulPos = rsPos;
		rightElbowPos = rePos;
		rightHandPos = rhPos;

		leftShoulPos = lsPos;
		leftElbowPos = lePos;
		leftHandPos = lhPos;

		rollLeftHand = rollLeft;
		rollRightHand = rollRight;
	}

	SkeletonInfo & operator=(const SkeletonInfo &si){
		neckPos = si.neckPos;

		rightShoulPos = si.rightShoulPos;
		rightElbowPos = si.rightElbowPos;
		rightHandPos = si.rightHandPos;

		leftShoulPos = si.leftShoulPos;
		leftElbowPos = si.leftElbowPos;
		leftHandPos = si.leftHandPos;

		rollLeftHand = si.rollLeftHand;
		rollRightHand = si.rollRightHand;

		return *this;
	}

};


//struct that takes each frame of skeleton information and transforms it into our feature vector
struct FeatureVector{

	//unit 3D vectors that we need as features
	JointPoint RS_RE;
	JointPoint RE_RH;
	
	JointPoint LS_LE;
	JointPoint LE_LH;

	JointPoint RH_LH;


	//angles that we need to get as features
	double N_RS_RE;
	double RS_RE_RH;
	double N_LS_LE;
	double LS_LE_LH;

	//distance from RH to LH that we need as a feature
	double distRHtoLH;

	//roll angles of hands as features
	double rollLeftHand;
	double rollRightHand;

	//the distance between the two festure vectors (how similar they are)
	double Dist(FeatureVector & featVect2){
		return RS_RE.Dist(featVect2.RS_RE) + RE_RH.Dist(featVect2.RE_RH) + LS_LE.Dist(featVect2.LS_LE) + LE_LH.Dist(featVect2.LE_LH)
			+ RH_LH.Dist(featVect2.RH_LH) + fabs(N_RS_RE - featVect2.N_RS_RE) + fabs(RS_RE_RH - featVect2.RS_RE_RH)
			+ fabs(N_LS_LE - featVect2.N_LS_LE) + fabs(LS_LE_LH - featVect2.LS_LE_LH) + fabs(distRHtoLH - featVect2.distRHtoLH)
			+ fabs(rollLeftHand - featVect2.rollLeftHand) + fabs(rollRightHand - featVect2.rollRightHand);
	}


	FeatureVector operator+(const FeatureVector &fv) const{
		FeatureVector featVect;

		featVect.RS_RE = RS_RE + fv.RS_RE;
		featVect.RE_RH = RE_RH + fv.RE_RH;
		featVect.LS_LE = LS_LE + fv.LS_LE;
		featVect.LE_LH = LE_LH + fv.LE_LH;
		featVect.RH_LH = RH_LH + fv.RH_LH;

		featVect.N_RS_RE = N_RS_RE + fv.N_RS_RE;
		featVect.RS_RE_RH = RS_RE_RH + fv.RS_RE_RH;
		featVect.N_LS_LE = N_LS_LE + fv.N_LS_LE;
		featVect.LS_LE_LH = LS_LE_LH + fv.LS_LE_LH;

		featVect.distRHtoLH = distRHtoLH + fv.distRHtoLH;

		featVect.rollLeftHand = rollLeftHand + fv.rollLeftHand;
		featVect.rollRightHand = rollRightHand + fv.rollRightHand;

		return featVect;
	}

	FeatureVector operator/(double div) const{
		FeatureVector featVect;

		featVect.RS_RE = RS_RE / div;
		featVect.RE_RH = RE_RH / div;
		featVect.LS_LE = LS_LE / div;
		featVect.LE_LH = LE_LH / div;
		featVect.RH_LH = RH_LH / div;

		featVect.N_RS_RE = N_RS_RE / div;
		featVect.RS_RE_RH = RS_RE_RH / div;
		featVect.N_LS_LE = N_LS_LE / div;
		featVect.LS_LE_LH = LS_LE_LH / div;

		featVect.distRHtoLH = distRHtoLH / div;

		featVect.rollLeftHand = rollLeftHand / div;
		featVect.rollRightHand = rollRightHand / div;

		return featVect;
	}


	//function to save a FeatureVector
	//saved as all the internal variables, one per line
	void Save(ofstream & fout){
		RS_RE.Save(fout);
		RE_RH.Save(fout);

		LS_LE.Save(fout);
		LE_LH.Save(fout);

		RH_LH.Save(fout);

		fout << N_RS_RE << endl;
		fout << RS_RE_RH << endl;
		fout << N_LS_LE << endl;
		fout << LS_LE_LH << endl;

		fout << distRHtoLH << endl;

		fout << rollLeftHand << endl;
		fout << rollRightHand;
		//^^not putting an endl above, we'll take care of this endl in CodeBook's Save
		//(cause we need to to read shit back properly)

	}

	//function to load a FeatureVector
	void Load(ifstream & fin){
		RS_RE.Load(fin);
		RE_RH.Load(fin);

		LS_LE.Load(fin);
		LE_LH.Load(fin);

		RH_LH.Load(fin);

		fin >> N_RS_RE;
		fin >> RS_RE_RH;
		fin >> N_LS_LE;
		fin >> LS_LE_LH;

		fin >> distRHtoLH;

		fin >> rollLeftHand;
		fin >> rollRightHand;

	}

};

//this class represents an observation sequence of a gesture
//each occurence of a gesture is an observation sequence
//each observation sequence is composed of a number of feature vectors
//each feature vector is composed of a list of skeleton features like angles between bones and unit vectors of bones
class ObservationSeq{

public:
	ObservationSeq(vector<SkeletonInfo> siList);

	int GetNumSkeletonSamples() const;

	vector<FeatureVector> & GetFeatureVectors();

	void SetClusterList(vector<int> clList);

	const vector<int> GetClusterList() const;


private:
	int numSkeletonSamples;

	//each SkeletonInfo in this list corresponds to a frame (or something similar) that's 
	//sampled for the skeleton information
	vector<SkeletonInfo> skeletonInfoList;

	//this is the list of our feature vectors that make up our observation sequence
	vector<FeatureVector> featureVectorList;

	//this list represents the cluster that each feature vector goes into
	vector<int> clusterList;

	//functions for calculating the features
	JointPoint CalculateRS_RE(SkeletonInfo & skeletonFrame);
	JointPoint CalculateRE_RH(SkeletonInfo & skeletonFrame);

	JointPoint CalculateLS_LE(SkeletonInfo & skeletonFrame);
	JointPoint CalculateLE_LH(SkeletonInfo & skeletonFrame);
	
	JointPoint CalculateRH_LH(SkeletonInfo & skeletonFrame);

	double CalculateN_RS_RE(JointPoint & RSRE, SkeletonInfo & skeletonFrame);
	double CalculateRS_RE_RH(JointPoint & RERH, SkeletonInfo & skeletonFrame);
	double CalculateN_LS_LE(JointPoint & LSLE, SkeletonInfo & skeletonFrame);
	double CalculateLS_LE_LH(JointPoint & LELH, SkeletonInfo & skeletonFrame);

	//distance from RH to LH that we need as a feature
	double CalculateDistRHtoLH(const SkeletonInfo & skeletonFrame);


	FeatureVector GetFeatureVector(SkeletonInfo & skeletonFrame);

	//function to get the distance of 2 points
	double Distance(const JointPoint & pt1, const JointPoint & pt2);

	//function to get the dot product
	double DotProduct(const JointPoint & pt1, const JointPoint & pt2);

};

#endif