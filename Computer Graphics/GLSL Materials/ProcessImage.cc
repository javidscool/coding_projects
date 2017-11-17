#include<fstream>
#include<iostream>
#include<se/graphics/image/formats/LdrImage.h>
#include<se/graphics/image/formats/TgaImage.h>
#include<se/graphics/image/FloatImageRGBA.h>
#include"BrdfHash.h"
#include<Eigen/Core>
#include<se/core/Serial.h>
using namespace std;
using namespace Eigen;
using namespace jcs::colorconverter;

Eigen::MatrixXi label_image(const se::graphics::image::FloatImageRGBA& fir,const std::vector<BrdfHash>& bhv)
{
	MatrixXi labels(fir.width,fir.height);

	for(int i=0;i<fir.height;i++)
	{
		for(int j=0;j<fir.width;j++)
		{
			se::graphics::image::Color4f cf=fir.read(i,j);

			Eigen::Vector3f cfvTemp = cf.asEigenCol().head<3>();

			Eigen::Vector3d cfv;
			cfv[0] = (double)cfvTemp[0];
			cfv[1] = (double)cfvTemp[1];
			cfv[2] = (double)cfvTemp[2];

			double curmin=1000000.0;
			int curminindex=0;
			for(int k=0;k<bhv.size();k++)
			{
				double d=(cfv.normalized() - bhv[k].meancolor.normalized()).norm();
				if(d < curmin)
				{
					curmin=d;
					curminindex=k;
				}
			}
			labels(j,i)=curminindex;
		}
	}

	return labels;
}

void print_help()
{
}


int main(int argc,char** argv)
{

	ifstream inhashfile("by.hash");

	int k;
	inhashfile >> k;
	cout << k << endl;
	std::vector<BrdfHash> hashes(k);

	for(int i=0;i<k;i++)
	{
		inhashfile >> hashes[i];
	}



    string test = "test.jpg.label";
    ifstream ifli("test.jpg",ifstream::in | ifstream::binary);
    se::graphics::image::formats::LdrImage li(ifli);
    se::graphics::image::FloatImageRGBA fir(li);
    MatrixXi labels=label_image(fir,hashes);
    ofstream ofl(test.c_str(),ofstream::out | ofstream::binary);
    se::graphics::image::FloatImageRGBA outfi(li);
    int c=labels.cols();
    int r=labels.rows();
    ofl < c;
    ofl < r;
    for(int j=0;j<labels.rows();j++){
        for(int k=0;k<labels.cols();k++)
        {
            se::graphics::image::Color4f c(se::math::Vector3D(hashes[labels(j,k)].meancolor.normalized().cast<float>().eval()));
            outfi.write(k,j,c);
            ofl < labels(j,k);	//row major output
        }
    }
    se::graphics::image::formats::LdrImage tg(outfi);
    tg.write(test + ".tga");



	return 0;
}

