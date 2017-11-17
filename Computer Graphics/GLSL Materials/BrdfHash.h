#ifndef __BRDFHASH_H
#define __BRDFHASH_H

#include<iosfwd>
#include<string>
#include<Eigen/Core>
namespace jcs
{
    namespace colorconverter
    {
        class Brdf;
        class BrdfHash
        {
            public:
                std::string brdfname;
                Eigen::Vector3d meancolor;
                Eigen::Vector3d stdcolor;

                BrdfHash(const std::string& name,const Brdf*);
                BrdfHash();

        };
    }
}


std::ostream& operator<<(std::ostream&,const jcs::colorconverter::BrdfHash&);
std::istream& operator>>(std::istream&,jcs::colorconverter::BrdfHash&);

#endif
