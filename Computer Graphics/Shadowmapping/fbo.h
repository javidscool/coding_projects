#ifndef FBO_H_
#define FBO_H_
#include <cstdlib>
#include <cstdio>
#include <iostream>
using std::cout;
using std::endl;
#include "TextureLoader.h"
#include "GL/glew.h"
#include "GL/glut.h"


class FBO
{
public:

	FBO(const CTexture &tex);

	// begin render to texture
	void beginRenderToTexture();
	void endRenderToTexture();

	// member variables
	GLuint fbo;
	GLuint tex;
	int width;
	int height;
};


#endif