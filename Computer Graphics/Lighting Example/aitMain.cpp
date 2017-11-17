//#include <assimp/assimp.h>
//#include <assimp/assimp.hpp>
//#include <assimp/aiTypes.h>
//#include <assimp/aiPostProcess.h>
//#include <assimp/aiScene.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include <se/core/CopyCounter.h>

#include <sstream>

#include <se/graphics/gl/Exceptions.h>
#include <se/core/Log.h>

#include "handyinlines.h"

#include "ScreenSpaceQuad.h"

#include <se/graphics/gl/PCGLWindow.h>
#include <se/sim/ElapsedTimer.h>
#include <se/sim/ui/PCInputBinding.h>
#include <iostream>
#include <se/graphics/image/formats/LdrImage.h>
#include <fstream>
#include <se/graphics/gl/OpenGLTextureObject.h>
#include <se/graphics/gl/GLSimpleShaderProgram.h>
#include <se/core/CopyCounter.h>
#include <se/graphics/gl/OpenGLFramebufferObject.h>
#include <se/graphics/gl/OpenGLTextureObject.h>

#include <se/graphics/DepthCamera.h>
#include <se/graphics/gl/FboDepthAttachment.h>
#include <se/graphics/gl/OpenGLDepthTexture.h>
#include <se/graphics/gl/FboStencilAttachment.h>
//#include <se/graphics/gl/FboColorAttachment.h>
#include <se/graphics/model/StaticMesh.h>
#include <se/sim/UpdateTimer.h>
#include "skybox.h"
#define log se::core::Log::status()

#include <Eigen/Geometry>
#include "ShooterCamera.h"

#include<se/graphics/image/formats/RgbeImage.h>

//using namespace Assimp;
using namespace std;
using namespace se::graphics::gl;
using namespace se::sim::ui;
using namespace se::graphics::image;


//todo:: try getting rid of stencil for main buffer, since all that shit's offscreen
//todo:: actually create and use lights
//todo:: create adjacency rendering mode

int main(int argc, char** argv)
{

    //log stuff
    se::core::Log::setVerbose(true);
    se::core::Log::status()<<"Running Vertex Buffer Model Importer"<<std::endl;


    //window crap
    std::cout<<"binding input bindings"<<std::endl;
    PCGLWindow::Settings settings;
    settings.width = 1024;
    settings.height = 1024;
    settings.glversion_major=3;
    settings.glversion_minor=0 ; //3.1,3.2,3.3 segfaults on windows gl drivers
    settings.is_floating_point = false;	// not yet tested under GLX, works in WGL
    settings.is_srgb = true;
    settings.is_antialiased = false;		// does not work in GLX (works in WGL)
    settings.antialias_amount = 2;
    settings.has_depth_buffer = true;
    settings.depth_buffer_bits = 24;
    settings.is_depth_buffer_floating_point = false;    // not yet tested under GLX, works in WGL
    settings.is_fullscreen = false; //full screen window
    settings.is_vsync = true;
    settings.has_stencil_buffer=true;
    settings.stencil_buffer_bits = 8;  //allocate an 8 bit stencil buffer
    std::cout<<"window settings set"<<std::endl;

    //create the window
    PCGLWindow window(settings);
    window.showPointer(true); //hide the cursor
    std::cout<<"window ctor done"<<std::endl;

    std::set<std::pair<int, int> > resolutions = window.getResolutions();
    for(std::set<std::pair<int, int> >::iterator it = resolutions.begin(); it != resolutions.end(); it++)
        cout << it->first << 'x' << it->second << endl;

    glEnable(GL_MULTISAMPLE);
    //  window.use_binding(binding);

    ShooterCamera dc(window); //create the shooter camera
    window.use_binding(dc);  //set the shooter camera as the window's input handler

    //more windowing stuff
    se::sim::ElapsedTimer et;
    window.bind();
    OpenGLRuntime& gl = window.getGLRuntime();

    unsigned frames = 0;
    try
    {
        //this is the only one you should actually be using in the final
        GLSimpleShaderProgram skyboxProg(gl, "skybox.vert", "skybox.frag");

        GLSimpleShaderProgram refractionProg(gl, "refractions.vert", "refractions.frag");

        GLSimpleShaderProgram reflectionProg(gl, "reflections.vert", "reflections.frag");

        GLSimpleShaderProgram diffEnvironProg(gl, "diffEnviron.vert","diffEnviron.frag");

        log<<"done creating shaders"<<std::endl;

        //setting up the background
        std::ifstream img("grace.tga", ios::in|ios::binary);
        formats::LdrImage pic(img);
        img.close();


        //setting up the diffuse environment map
        std::ifstream imgDiff("gracediff.tga", ios::in|ios::binary);
        formats::LdrImage picDiff(imgDiff);
        imgDiff.close();


        //loading in the texture of the robot
        std::ifstream imgRobot("Robot.png", ios::in|ios::binary);
        formats::LdrImage picRobot(imgRobot);
        imgRobot.close();


        gl.getUnits().getUnit(3).bind();
        se::graphics::gl::OpenGLTextureObject skytex(gl, pic);
        skytex.bind();
        skytex.setTextureFilter(OpenGLTextureObject::FILTER_BILINEAR);

        gl.getUnits().getUnit(4).bind();
        se::graphics::gl::OpenGLTextureObject skyDifftex(gl, picDiff);
        skyDifftex.bind();
        skyDifftex.setTextureFilter(OpenGLTextureObject::FILTER_BILINEAR);

        gl.getUnits().getUnit(5).bind();
        se::graphics::gl::OpenGLTextureObject robotTex(gl, picRobot);
        robotTex.bind();
        robotTex.setTextureFilter(OpenGLTextureObject::FILTER_BILINEAR);


        gl.getUnits().getUnit(0).bind();


        glBindTexture(GL_TEXTURE_2D, 0);



        std::string infilename = "vase0.vbo";
        std::ifstream intest2(infilename.c_str(), std::ios::in | std::ios::binary);
        StaticMesh vasemesh(intest2);


        std::ifstream intest4(infilename.c_str(), std::ios::in | std::ios::binary);
        StaticMesh vasemesh2(intest4);

        ScreenSpaceQuad ssq;

        intest2.close();
        intest4.close();

        glClearColor(0.0f,0.0f,1.0f,1.0f);



        std::ifstream intest3("lego0.vbo", std::ios::in|std::ios::binary);
        StaticMesh legomesh( intest3);
        intest3.close();


        std::string canfile = "can0.vbo";
        std::ifstream cantest(canfile.c_str(), std::ios::in | std::ios::binary);
        StaticMesh canmesh(cantest);

        SkyBox sky;

        while(dc.running)
        {



            glClearColor(0.0,0.0,0.0,1.0);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            //glViewport(0,0,1440,900);
            //setup geometry rendering
            glDisable(GL_STENCIL_TEST); // don't need stencil testing for gbuffer generation passes
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear all the buffers
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            glEnable(GL_DEPTH_TEST);

            //***********transformation code for objects.*************
            static double accum = 0;
            accum += .01;
            Eigen::Matrix4f vasetransform = translation(-1.5,-0.50,-5.0) * scale(.0075,.0075,.0075);
            Eigen::Matrix4f modelVase = vasetransform;

            Eigen::Matrix4f vasetransform2 = translation( 1.5,0.0,-5.0) * scale(.0075,.0075,.0075) * rotateY(accum);
            Eigen::Matrix4f modelVase2 = vasetransform2;


            Eigen::Matrix4f cantransform = translation(0.25,-0.50,-3.0) * rotateY(accum) * rotateX(0.0) * scale(.15,.15,.15);
            Eigen::Matrix4f modelCan = cantransform;


            Eigen::Matrix4f legotransform = translation( 1.5,0.0,-5.0) * scale(.5,.5,.5) * rotateY(accum);
            Eigen::Matrix4f modelLego = legotransform;





            vasemesh.setModelMatrix(modelVase);

            vasemesh2.setModelMatrix(modelVase2);

            canmesh.setModelMatrix(modelCan);

            legomesh.setModelMatrix((modelLego));

            //generate normal matrices
            {
                Eigen::Matrix3f normalmatrixvase = (modelVase).topLeftCorner<3,3>().inverse().transpose();
                vasemesh.setNormalMatrix(normalmatrixvase);
            }

            {
                Eigen::Matrix3f normalmatrixvase2 = (modelVase2).topLeftCorner<3,3>().inverse().transpose();
                vasemesh2.setNormalMatrix(normalmatrixvase2);
            }

            {
                Eigen::Matrix3f normalmatrixcan = (modelCan).topLeftCorner<3,3>().inverse().transpose();
                canmesh.setNormalMatrix(normalmatrixcan);
            }

            {
                Eigen::Matrix3f normalmatrixlego = (modelLego).topLeftCorner<3,3>().inverse().transpose();
                legomesh.setNormalMatrix(normalmatrixlego);
            }


            et.update();

            ////////////////////////////////////
            //doing all the skybox shader shit//
            ////////////////////////////////////
            skyboxProg.bind();

            Eigen::Matrix4f proj = projection();
            Eigen::Matrix4f inverseCameraMatrix = dc.getMatrix().inverse();

            //projection * view matrix
            //takes things from world space to clip space
            Eigen::Matrix4f vm = proj  * dc.getMatrix();

            glUniformMatrix4fv(glGetUniformLocation(skyboxProg.getID(), "projection"), 1, GL_FALSE, vm.data());

            //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            //sending the skybox texture to the vert and frag shaders
            skyboxProg.setTexture("texture", gl.getUnits().getUnit((3)));
            sky.Render(gl, skyboxProg);

            ////////////////////////////////////
            //doing all the skybox shader shit//
            ////////////////////////////////////

            //enable these settings cause now i'm working with objects
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);


            ////////////////////////////////////////
            //doing all the reflection shader shit//
            ////////////////////////////////////////
            reflectionProg.bind();

            //projection matrix of can
            Eigen::Matrix4f projectionMatrixCan = proj;


            //passing projection matrix and modelview matrix of the can
            glUniformMatrix4fv(glGetUniformLocation(reflectionProg.getID(), "projmatrix"), 1, GL_FALSE, projectionMatrixCan.data());
            glUniformMatrix4fv(glGetUniformLocation(reflectionProg.getID(), "invcameramatrix"), 1, GL_FALSE, inverseCameraMatrix.data());
            glUniformMatrix4fv(glGetUniformLocation(reflectionProg.getID(), "cameramatrix"), 1, GL_FALSE, dc.getMatrix().data());

            reflectionProg.setTexture("texture", gl.getUnits().getUnit((3)));

            canmesh.Render(gl, reflectionProg);

            ////////////////////////////////////////
            //doing all the reflection shader shit//
            ////////////////////////////////////////



            ////////////////////////////////////////
            //doing all the refraction shader shit//
            ////////////////////////////////////////
            refractionProg.bind();

            //projection matrix of can
            Eigen::Matrix4f projectionMatrixVase = proj;

            //passing projection matrix and modelview matrix of the can
            glUniformMatrix4fv(glGetUniformLocation(refractionProg.getID(), "projmatrix"), 1, GL_FALSE, projectionMatrixVase.data());
            glUniformMatrix4fv(glGetUniformLocation(refractionProg.getID(), "invcameramatrix"), 1, GL_FALSE, inverseCameraMatrix.data());
            glUniformMatrix4fv(glGetUniformLocation(refractionProg.getID(), "cameramatrix"), 1, GL_FALSE, dc.getMatrix().data());


            refractionProg.setTexture("texture", gl.getUnits().getUnit((3)));

            vasemesh.Render(gl, refractionProg);

            ////////////////////////////////////////
            //doing all the refraction shader shit//
            ////////////////////////////////////////


            ///////////////////////////////////////////////////
            //doing all the diffuse environment lighting shit//
            ///////////////////////////////////////////////////
            diffEnvironProg.bind();

            //projection matrix of can
            //Eigen::Matrix4f projectionMatrixLego = proj;
            Eigen::Matrix4f projectionMatrixVase2 = proj;

            //passing projection matrix and modelview matrix of the can
            //glUniformMatrix4fv(glGetUniformLocation(diffEnvironProg.getID(), "projmatrix"), 1, GL_FALSE, projectionMatrixLego.data());
            glUniformMatrix4fv(glGetUniformLocation(diffEnvironProg.getID(), "projmatrix"), 1, GL_FALSE, projectionMatrixVase2.data());
            glUniformMatrix4fv(glGetUniformLocation(diffEnvironProg.getID(), "cameramatrix"), 1, GL_FALSE, dc.getMatrix().data());


            //putting in the skybox texture
            diffEnvironProg.setTexture("diffTexture", gl.getUnits().getUnit((4)));

            diffEnvironProg.setTexture("robotTexture", gl.getUnits().getUnit((5)));

            //legomesh.Render(gl, diffEnvironProg);
            vasemesh2.Render(gl, diffEnvironProg);
            ///////////////////////////////////////////////////
            //doing all the diffuse environment lighting shit//
            ///////////////////////////////////////////////////



            frames++;
            window.update(et.getElapsedSeconds());
            window.flush();
        }
    }

    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }
    cout << "average spf:" << (5.0 / frames) << endl;
    cout << "average fps:" << (frames/ 5.0) << endl;




    //system("Pause");

    return 0;
}

