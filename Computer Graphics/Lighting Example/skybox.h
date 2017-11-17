#ifndef VIRTUOSO_SKYBOX_H
#define VIRTUOSO_SKYBOX_H
#include <se/graphics/gl/OpenGLRuntime.h>
#include <se/core/CopyCounter.h>
#include <se/graphics/gl/GLSimpleShaderProgram.h>

class SkyBox{
private:
se::core::CopyCounter copycount;
GLuint vertex_array;
GLuint indexbuffer;
GLuint vbo;

public:

SkyBox();
virtual ~SkyBox();

void Render(se::graphics::gl::OpenGLRuntime&, se::graphics::gl::GLSimpleShaderProgram& shader);

};

#endif
