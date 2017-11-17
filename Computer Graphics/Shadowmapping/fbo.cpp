#include "fbo.h"

FBO::FBO(const CTexture &tex)
{
	this->tex = tex.tex;
	this->width = tex.width;
	this->height = tex.height;

    //generating a framebuffer object name by creating 1 framebuffer object name with fbo pointing to it
	glGenFramebuffersEXT(1, &this->fbo);

    //binding the framebuffer object name (fbo) to a framebuffer target (GL_FRAMEBUFFER)
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo); 
    
	//allocating a renderbuffer object name
	GLuint depthbuffer; 

	//generating a renderbuffer object name by creating 1 renderbuffer object name with depthbuffer pointing to it
	glGenRenderbuffersEXT(1, &depthbuffer); 

    //binding the renderbuffer object name (depthbuffer) to the renderbuffer target (GL_RENDERBUFFER)
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer); 

    //create data storage and establish the pixel format (GL_DEPTH_COMPONENT) and dimensions (width and height) of the renderbuffer image
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

    //attach the renderbuffer to a framebuffer target
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer/*name of the renderbuffer object that you want to attach to the framebuffer*/); 

}


// begin render to texture
void FBO::beginRenderToTexture()
{
	//attach the texture to the framebuffer 
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
    
	//binding the framebuffer object name (fbo) to the framebuffer target (GL_FRAMEBUFFER)
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->fbo);

    
	//indicates which groups of state variables to save on the attribute stack
	//in this case save Depth range (near and far), the viewport origin and extent
	glPushAttrib(GL_VIEWPORT_BIT);

	//maps normalized device coordinates to window coordinates
	glViewport(0,0,width, height);

}

// end render to texture
void FBO::endRenderToTexture()
{
	//restores the values of the state variables saved with the last glPushAttrib command
	glPopAttrib();

	//binding nothing to the framebuffer target (GL_FRAMEBUFFER), basically clearing the framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//binding texture name tex to texturing target GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, this->tex);
}