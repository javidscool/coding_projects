#include "Brdf.h"
#include <se/core/Exception.h>
#include <se/core/Serial.h>
#include <se/core/open_mp.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <se/core/Log.h>
#include <iostream>
#include <fstream>
#include <se/core/Filesystem.h>
using namespace std;

using namespace se::core;
using namespace jcs::colorconverter;


typedef Eigen::Vector3d	Color3d;
typedef boost::function<Color3d (const double&,const double&,const double&,const double&)> BrdfLooker;

inline static double max(const double& a,const double& b)
{
	return (a > b ? a : b);
}

template<class M>
void brdf_normalize(M& bdata,const int& p)
{
	Eigen::VectorXd weights(bdata.rows());
	Eigen::VectorXd norms(bdata.rows());
	double* wraw=weights.data();

	for(int i=1;i<=p;i++)
	{
		double pi=(M_PI/2.0)*static_cast<double>(i)/static_cast<double>(p);
		double tconst=max(sin(pi),0.0); // /(2.0*M_PI);? constant here for proper spherical area?
		//double tconst=1.0;
		for(int j=1;j<=4*p;j++)
		{
			int dex=((i-1)*p*4) + (j-1);
			//double ti=2.0*M_PI*static_cast<double>(j)/static_cast<double>(p*2);
			wraw[dex]=tconst;
		}
	}

	norms=bdata*weights;
	for(int i=0;i<norms.rows();i++)
	{
		if(norms[i] > 1.0)
		{
			bdata.row(i) /= norms[i];
		}
	}
}
void Brdf::normalize()
{
	Log::status() << "Normalizing brdf data.." << endl;
	SOMP_PARALLEL_FOR_L2
	for(int i=0;i<3;i++) brdf_normalize(internalBrdfData[i],hemisphere_dimension);
}

void Brdf::logalize()
{
	Log::status() << "Logalizing brdf data.." << endl;
	for(int i=0;i<3;i++)
	{
		double* datp=internalBrdfData[i].data();
		int ds=internalBrdfData[i].rows()*internalBrdfData[i].cols();
		SOMP_PARALLEL_FOR_L2
		for(int j=0;j<ds;++j)
		{
			datp[j]=std::log(datp[j]);
		}
	}
}

void Brdf::unlogalize()
{
	Log::status() << "Unlogalizing brdf data.." << endl;
	for(int i=0;i<3;i++)
	{
		double* datp=internalBrdfData[i].data();
		int ds=internalBrdfData[i].rows()*internalBrdfData[i].cols();
		SOMP_PARALLEL_FOR_L2
		for(int j=0;j<ds;++j)
		{
			datp[j]=std::exp(datp[j]);
		}
	}
}

Brdf::Brdf(std::istream& in,const std::uint32_t& hd,const fileformat& ff):
	hemisphere_dimension(hd)
{
	switch(ff)
	{
	case SOAPER:
		load_soaper(in);
		break;
	case MERL:
		load_merl(in);
		break;
	default:
		throw Exception("Error, fileformat encoding not supported yet!");
	};
}

Brdf::Brdf(const std::uint32_t& hd):
	hemisphere_dimension(hd)
{
	initialize_brdf_data();
	const std::uint32_t N=hemisphere_dimension*hemisphere_dimension*4;
	SOMP_PARALLEL_FOR_L3
	for(int i=0;i<3;i++)
		memset((void*)internalBrdfData[i].data(),0,N*N*sizeof(double));
}


void sample_init(Brdf* b,const BrdfLooker& bl);
void Brdf::load_soaper(std::istream& in)
{
	//std::uint32_t requested_hd=hemisphere_dimension;
	//Log::status() << "Reading brdf from stream.." << endl;
	in > hemisphere_dimension;
	std::uint32_t N=initialize_brdf_data();

	for(int i=0;i<3;i++)
		in.read(reinterpret_cast<char*>(internalBrdfData[i].data()),N*N*sizeof(double));
}
//void load_merl(std::istream& in);
//void load_lua(std::istream& in);
//void load_script(std::istream& in);
std::ostream& operator<(std::ostream& out,const Brdf& b)
{
	const std::uint32_t N=b.getHemisphereDimension()*b.getHemisphereDimension()*4;
	out < b.getHemisphereDimension();
	for(int i=0;i<3;i++)
		out.write(reinterpret_cast<const char*>(b.getInternalData(i).data()),N*N*sizeof(double));
	return out;
}

double* read_brdf(std::istream& in);
void lookup_brdf_val(double* brdf, double theta_in, double fi_in,
			  double theta_out, double fi_out,
			  double& red_val,double& green_val,double& blue_val);

Color3d merl_lookup(double* brdfmerl,const double& ti,const double& pi,const double& to,const double& po)
{
	double r,g,b;
	//for the merl lookup brdf val, swap theta and phi coordinate systems
	lookup_brdf_val(brdfmerl,pi,ti,po,to,r,g,b);
	return Color3d(r,g,b);
}
void Brdf::load_merl(std::istream& in)
{

// Read BRDF data
	//Log::status() << "Reading brdf database from the stream.." << endl;
	double* brdf=read_brdf(in);
	initialize_brdf_data();
	sample_init(this,boost::bind(merl_lookup,brdf,_1,_2,_3,_4));
	delete [] brdf;
}


std::uint32_t Brdf::initialize_brdf_data()
{
	//Log::status() << "Allocating memory for brdf matrices.." << endl;
	const std::uint32_t N=hemisphere_dimension*hemisphere_dimension*4;
	SOMP_PARALLEL_FOR_L3
	for(int i=0;i<3;i++)
		internalBrdfData[i]=Eigen::MatrixXd((int)N,(int)N);

	return N;
}
void sample_init(Brdf* b,const BrdfLooker& bl)
{
	Log::status() << "Initializing brdf samples.." << endl;
	int p=b->getHemisphereDimension();
	for(int k=1;k<=p;k++)
	{
		double po=(M_PI/2.0)*static_cast<double>(k)/static_cast<double>(p);
		for(int l=1;l<=p*4;l++)
		{
			double to=2.0*M_PI*static_cast<double>(l)/static_cast<double>(p*4);
			int dexo=((k-1)*p*4)+(l-1);
			SOMP_PARALLEL_FOR_L2
			for(int i=1;i<=p;i++)
			{
				double pi=(M_PI/2.0)*static_cast<double>(i)/static_cast<double>(p);
				//double tconst=sin(pi)*cos(pi); // /(2.0*M_PI);? constant here for proper spherical area?
				double tconst=1.0;
				for(int j=1;j<=4*p;j++)
				{
					int dex=((i-1)*p*4) + (j-1);
					double ti=2.0*M_PI*static_cast<double>(j)/static_cast<double>(p*4);
					Color3d brdfcolor=bl(ti,pi,to,po);
					for(int c=0;c<3;c++) b->getInternalData(c)(dex,dexo)=brdfcolor[c];
					//b->getInternalData(0)(dex,dexo)=brdfcolor[0];
				}
			}
		}
	}
}

namespace jcs
{
namespace colorconverter
{

Brdf* load_brdf(const string& input_file,const std::uint32_t& p)
{
	string ext=Filesystem::get_extension(input_file);

	ifstream infile(input_file.c_str(),ifstream::in | ifstream::binary);

	if(!infile)
		throw Exception(string("Error reading file ")+input_file);

	Brdf* brdfp;
		//Brdf::fileformat ff;
	if(ext=="tbrdf" || ext=="bbrdf")
	{
		brdfp=new Brdf(infile,p,Brdf::SOAPER);
	}
	else if(ext=="binary")
	{
		brdfp=new Brdf(infile,p,Brdf::MERL);
	}
	else if(ext=="lua" || ext=="lb")
	{
		brdfp=new Brdf(infile,p,Brdf::LUA);
	}
	else if(ext=="bscript")
	{
		brdfp=new Brdf(infile,p,Brdf::BSCRIPT);
	}
	else
	{
		throw Exception("input file extension not found or not supported!");
	}
	infile.close();
	return brdfp;

}
}
}
