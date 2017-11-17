#include "fbo.h"

FBO::FBO()
{
	glGenFramebuffersEXT(1, &this->fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);

	glGenRenderbuffersEXT(1, &(this->depthbuffer));

	currentIndex = 0;
}

void FBO::setTexture(int index, const CTexture& tex)
{
	if ((index >= 0) && (index < MAX_TEXTURES))
	{
		this->width[index] = tex.width;
		this->height[index] = tex.height;
		this->tex[index] = tex.tex;
	}
	else
	{
		cout << "Invalid texture index" << endl;
	}
}


// begin render to texture
void FBO::beginRenderToTexture(int index)
{
	if ((index >= 0) && (index < MAX_TEXTURES))
	{
		this->currentIndex = index;

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->depthbuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, this->width[index], this->height[index]);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->depthbuffer);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);

		// attach texture
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
			this->tex[index], 0);

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, this->width[index], this->height[index]);
	}
	else
	{
		cout << "Invalid texture index" << endl;
	}
}

// end render to texture
void FBO::endRenderToTexture()
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glActiveTexture(GL_TEXTURE1 + currentIndex);
	glBindTexture(GL_TEXTURE_2D, tex[currentIndex]);

	currentIndex = 0;
}