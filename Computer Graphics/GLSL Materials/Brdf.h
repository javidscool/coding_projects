#ifndef __AOBB_TOOLS_BRDF_H
#define __AOBB_TOOLS_BRDF_H

#include<iosfwd>
#include<se/core/Types.h>
#include<Eigen/Core>
#include<string>
namespace jcs
{
namespace colorconverter
{

class Brdf
{
public: 
	enum fileformat
	{
		SOAPER,LUA,BSCRIPT,MERL
	};
	void normalize();
	void logalize();
	void unlogalize();
	const std::uint32_t& getHemisphereDimension() const;
	const Eigen::MatrixXd& getInternalData(const unsigned char& channel) const;
	Eigen::MatrixXd& getInternalData(const unsigned char& channel);
	//const Eigen::MatrixXd[3]& getInternalData();
	Brdf(std::istream& in,const std::uint32_t& hd=32,const fileformat& ff=Brdf::SOAPER);
	Brdf(const std::uint32_t& hd=32);
protected:
	std::uint32_t hemisphere_dimension;
	Eigen::MatrixXd internalBrdfData[3];	//red,green,blue
	std::uint32_t initialize_brdf_data();
	
	void load_soaper(std::istream& in);
	void load_merl(std::istream& in);
//	void load_lua(std::istream& in);
//	void load_script(std::istream& in);

//	friend std::ostream& operator<(std::ostream&,const Brdf&);
};

Brdf* load_brdf(const std::string&,const std::uint32_t& p=32);
}
}
std::ostream& operator<(std::ostream&,const jcs::colorconverter::Brdf&);
#include "Brdf.tcc"
#endif
