#include "skybox.h"

void SkyBox::Render(se::graphics::gl::OpenGLRuntime&, se::graphics::gl::GLSimpleShaderProgram& shader)
{
    bool oldcull = glIsEnabled(GL_CULL_FACE);
    bool olddepth = glIsEnabled(GL_DEPTH_TEST);
    GLboolean oldwrite = false;

    glGetBooleanv(GL_DEPTH_WRITEMASK, &oldwrite);

    if(oldcull)glDisable(GL_CULL_FACE);
    if(olddepth)glDisable(GL_DEPTH_TEST);
    if(oldwrite)glDepthMask(GL_FALSE);

    //bind the shader we pass in
    shader.bind();

    //bind the geometry we want to use
    glBindVertexArray(vertex_array);

    glBindAttribLocation(shader.getID(), 0, "position");

    // glLinkProgram(shader.getID());

    glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, 0);


    if(oldcull)glEnable(GL_CULL_FACE);
    if(olddepth)glEnable(GL_DEPTH_TEST);
    if(oldwrite)glDepthMask(GL_TRUE);
}


SkyBox::~SkyBox()
{
    if(copycount.unique()) {
        glBindVertexArray(0);

        glDeleteBuffers(1, &vbo);

        glDeleteVertexArrays(1, & vertex_array);

        glDeleteBuffers(1, & indexbuffer);
    }
}



SkyBox::SkyBox()
{

    GLfloat vertices[24] = {

        -1000.0f, 1000.0f, 1000.0f, //upper left back
        1000.0f, 1000.0f, 1000.0f, //upper right back
        1000.0f, 1000.0f, -1000.0f, //upper right front
        -1000.0f, 1000.0f, -1000.0f, //upper left front

        -1000.0f, -1000.0f, 1000.0f, //lower left back
        1000.0f, -1000.0f, 1000.0f, //lower right back
        1000.0f, -1000.0f, -1000.0f, //lower right front
        -1000.0f, -1000.0f, -1000.0f, //lower left front


    };


//specified in clockwise order wrt the outside of the box; we want to see inside
    GLuint indices[36] = {

        //top face triangles
        0, 2, 3,
        0, 1, 2,

        //bottom face triangles
        4, 7, 6,
        5, 4, 6,

        //front face triangles
        2, 6, 3,
        3, 6, 7,

        //back face triangles
        1, 0, 4,
        1, 5, 4,

        //right face
        1, 5, 6,
        2, 1, 6,

        //left face
       0,3,4,
       3,7,4

    };


    glGenVertexArrays(1, &vertex_array);

    glGenBuffers(1, & indexbuffer);

    if(!vertex_array || !indexbuffer) {
        throw se::graphics::gl::ResourceAllocationException("could not allocate VBO or vertex array");
    }

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

    //create an empty vbo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*36, indices, GL_STATIC_DRAW);


    glGenBuffers(1, &vbo);

    if(!vbo)throw se::graphics::gl::ResourceAllocationException("Could not allocate attribute buffer");


    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);

}


