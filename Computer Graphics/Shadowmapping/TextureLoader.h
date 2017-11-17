#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_
#include <cstdlib>
#include <cstdio>
#include <iostream>
using std::cout;
using std::endl;

#include "GL/glew.h"
#include "GL/glut.h"


bool loadPPM(char *fileName, int &width, int &height, unsigned char* &pixels);

class CTexture
{
public:

	CTexture(){ width = 0; height = 0; pixels = NULL; };
	~CTexture(){ if(pixels!=NULL) {delete [] pixels;} }

	bool loadPPM(char *fileName);
	bool savePPM(char *fileName);

	void createRT(int width, int height);

	int width;
	int height;
	GLuint tex;
	unsigned char* pixels;
};

#endif