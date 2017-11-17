#include<iostream>
#include"Brdf.h"
#include<vector>
#include"BrdfHash.h"
#include<string>
#include<fstream>
using namespace std;
using namespace jcs::colorconverter;

void print_help()
{
	cout << "Usage: " << "<outputfile> <brdf1> <brdf2> <brdf3>";

}

int main(int argc,char** argv)
{
	if(argc < 2)
	{
		print_help();
		return 0;
	}

	std::vector<string> args(argv,argv+argc);

	ofstream outhashfile(argv[1]);

	int k=args.size()-2;
	outhashfile << k <<endl;

	for(int i=2;i<args.size();i++)
	{
		Brdf* bp=load_brdf(argv[i]);
		BrdfHash bh(argv[i],bp);
		outhashfile << bh << endl;
		delete bp;
	}
	return 0;
}
