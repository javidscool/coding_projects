namespace jcs
{
namespace colorconverter
{

inline const std::uint32_t& Brdf::getHemisphereDimension() const
{
	return hemisphere_dimension;
}

inline const Eigen::MatrixXd& Brdf::getInternalData(const unsigned char& channel) const
{
	return internalBrdfData[channel];
}

inline Eigen::MatrixXd& Brdf::getInternalData(const unsigned char& channel)
{
	return internalBrdfData[channel];
}


}
}
