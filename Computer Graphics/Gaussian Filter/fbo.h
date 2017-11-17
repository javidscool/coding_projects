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

#define MAX_TEXTURES 8

class FBO
{
public:

	FBO();

	// begin render to texture
	void beginRenderToTexture(int textureIndex);
	void endRenderToTexture();

	void setTexture(int index, const CTexture& tex);

	// member variables
	GLuint fbo;
	GLuint depthbuffer;
	GLuint tex[MAX_TEXTURES];
	int width[MAX_TEXTURES];
	int height[MAX_TEXTURES];

	int currentIndex;
};


#endif