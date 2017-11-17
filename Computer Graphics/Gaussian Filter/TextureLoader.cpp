#include "TextureLoader.h"

bool loadPPM(char *fileName, int &width, int &height, unsigned char* &pixels)
{
	FILE *fp = NULL;
	fp = fopen(fileName, "rb");
	if(!fp) return false;

	// skip two lines
	while(fgetc(fp)!='\n') continue;
	while(fgetc(fp)!='\n') continue;
	// read width and height
	fscanf(fp, "%i %i", &width, &height);
	// read max val
	int max_val;
	fscanf(fp,"%i",&max_val);
	// allocate space for pixels
	pixels = new unsigned char[width * height * 3];
	for(int i=0;i<width*height*3;i++)
	{
		int _val;
		fscanf(fp,"%i",&_val);
		pixels[i] = static_cast<unsigned char>(_val);
	}
	
	fclose(fp);


	return true;
}

// Texture class

bool CTexture::loadPPM(char *fileName)
{
	// load from file
	if(!::loadPPM(fileName,this->width, this->height, this->pixels))
		return false;

	// OpenGL stuff
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->pixels);

	return true;
}

// create render target texture
void CTexture::createRT(int width, int height)
{
	this->width = width;
	this->height = height;

	// OpenGL stuff
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

// save PPM
bool CTexture::savePPM(char *fileName)
{
	FILE* fp = fopen(fileName, "w");
	if(!fp) return false;

	// write headers
	fprintf(fp,"P3\n");
	fprintf(fp,"# I made this\n");
	fprintf(fp,"%i %i\n", this->width, this->height);
	fprintf(fp,"255\n");
	for(int i=0;i<width*height*3;i++)
		fprintf(fp,"%i ",static_cast<int>(this->pixels[i]));

	fclose(fp);

	return true;
}