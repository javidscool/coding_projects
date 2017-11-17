#include"BrdfHash.h"
#include"Brdf.h"
#include<iostream>
using namespace std;
using namespace jcs::colorconverter;


//This calculates the BRDF features
BrdfHash::BrdfHash(const std::string& tn,const Brdf* bp):
	brdfname(tn)
{
	//color channel
	for(int c=0;c<3;c++)
	{
		const Eigen::MatrixXd& bd=bp->getInternalData(c);
		double sm=0.0;
		int p=bp->getHemisphereDimension();
		//phi out
		for(int k=1;k<=p;k++)
		{
			double po=(M_PI/2.0)*static_cast<double>(k)/static_cast<double>(p);
			//theta out
			for(int l=1;l<=p*4;l++)
			{
				double to=2.0*M_PI*static_cast<double>(l)/static_cast<double>(p*4);
				int dexo=((k-1)*p*4)+(l-1);
				for(int i=1;i<=p;i++)
				{
					double pi=(M_PI/2.0)*static_cast<double>(i)/static_cast<double>(p);
					//double tconst=sin(pi); // /(2.0*M_PI);? constant here for proper spherical area?
					double tconst=1.0;
					for(int j=1;j<=4*p;j++)
					{
						int dex=((i-1)*p*4) + (j-1);
						double ti=2.0*M_PI*static_cast<double>(j)/static_cast<double>(p*4);
						sm+=bd(dex,dexo)*tconst;
					}
				}
			}
		}
		meancolor[c]=sm/(16.0*static_cast<double>(p*p*p*p));

	}
}
BrdfHash::BrdfHash()
{
}

std::ostream& operator<<(std::ostream& out,const jcs::colorconverter::BrdfHash& bh)
{
	out << bh.brdfname << "\t"
		<< bh.meancolor[0] << "\t" << bh.meancolor[1] << "\t" << bh.meancolor[2] << "\t";
	return out;
}
std::istream& operator>>(std::istream& in,jcs::colorconverter::BrdfHash& bh)
{
	in 	>> bh.brdfname
		>> bh.meancolor[0] >> bh.meancolor[1] >> bh.meancolor[2];
	return in;
}
