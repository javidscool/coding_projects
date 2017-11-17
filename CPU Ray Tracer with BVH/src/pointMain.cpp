#include <se/graphics/gl/PCGLWindow.h>
#include <se/sim/ui/PCInputBinding.h>
#include <se/graphics/gl/GLSimpleShaderProgram.h>
#include "transformations.h"
#include "ShooterCamera.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include <se/core/CopyCounter.h>

#include <sstream>

//#include <se/graphics/gl/Exceptions.h>
#include <se/core/Log.h>

#include "Mesh.h"

#include "handyinlines.h"

//#include "ScreenSpaceQuad.h"

//#include <se/graphics/gl/PCGLWindow.h>
#include <se/sim/ElapsedTimer.h>
#include <iostream>
#include <se/graphics/image/formats/LdrImage.h>
#include <fstream>
//include <se/graphics/gl/GLSimpleShaderProgram.h>
#include <se/graphics/image/FloatImageRGBA.h>
#include <se/core/CopyCounter.h>

#include <se/sim/UpdateTimer.h>
#include "transformations.h"
#define log se::core::Log::status()

#include <Eigen/Geometry>

//#include "ShooterCamera.h"

#include "PointTracer.h"
#include "Misc.h"
#include <cfloat>
#include <algorithm>




using namespace std;
using namespace se::graphics::gl;
using namespace se::graphics::image;


//you define this.  controls window dimensions, creates framebuffer for ray tracing
//const unsigned int WINDOW_WIDTH = 512;
//const unsigned int WINDOW_HEIGHT = 512;

const unsigned int WINDOW_WIDTH = 1440;
const unsigned int WINDOW_HEIGHT = 900;
//typedef GLubyte pixel[3];


int main(int argc, char** argv)
{

    se::core::Log::setVerbose(true);

    //create the window and gl context
    PCGLWindow::Settings settings;
    settings.width = WINDOW_WIDTH;
    settings.height = WINDOW_HEIGHT;
    settings.glversion_major=3;
    settings.glversion_minor=0 ; //3.1,3.2,3.3 segfaults on windows gl drivers
    settings.is_floating_point = false;	// not yet tested under GLX, works in WGL
    settings.is_srgb = true;
    settings.is_antialiased = false;		// does not work in GLX (works in WGL)
    settings.antialias_amount = 2;
    settings.has_depth_buffer = true;
    settings.depth_buffer_bits = 8;
    settings.is_depth_buffer_floating_point = false;    // not yet tested under GLX, works in WGL
    settings.is_fullscreen = false;
    settings.is_vsync = true;

    std::cout<<"window settings set"<<std::endl;

    PCGLWindow window(settings);


    std::cout<<"window ctor done"<<std::endl;


    ShooterCamera dc(window);

    window.use_binding(dc);

    se::sim::ElapsedTimer et;

    window.bind();

    OpenGLRuntime& gl = window.getGLRuntime();

    unsigned frames = 0;


    srand(time(0));
    std::vector<PointObject> objectList;

    Eigen::Vector3f col(1.0f, 0.0f, 0.0f);
    PointObject sphere(col, 0.0f, -3.5f, -12.0f, 4.0f);
    objectList.push_back(sphere);

    //creating the PointTracer
    PointTracer pointTracer(objectList, WINDOW_WIDTH, WINDOW_HEIGHT);

    try{
        GLSimpleShaderProgram pointProg(gl, "point.vert", "point.frag");

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);

        pointProg.bind(gl);

        Eigen::Matrix4f projectionMat = projection();



        while(dc.running){
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            Eigen::Matrix4f modelViewProj = projectionMat * dc.getMatrix();

            glUniformMatrix4fv(glGetUniformLocation(pointProg.getID(), "modelviewProjection"), 1, GL_FALSE, modelViewProj.data());


            glColor3f(0.0f, 1.0f, 0.0f);

            //setting point size of points
            glPointSize(3);


            glBegin(GL_POINTS);

            for(int i = 0; i < sphere.pointList.size(); i++){
                //log << "sphere list size: " << sphere.pointList.size() << endl;
                glVertex3f(sphere.pointList[i].position[0], sphere.pointList[i].position[1], sphere.pointList[i].position[2]);
            }


            Eigen::Vector3f centroid = sphere.SetCentroid();

            glEnd();



            glPointSize(15);
            glColor3f(1.0f, 1.0f, 1.0f);



            glBegin(GL_POINTS);

            glVertex3f(centroid[0], centroid[1], centroid[2]);

            glEnd();


            //getting new coord system
            pointTracer.SetCoordSystem(sphere, centroid, 0);


            //x-axis is red, y-axis is green, z-axis is blue


            glLineWidth(15);

            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex3f(centroid[0], centroid[1], centroid[2]);
            glVertex3f(centroid[0] + pointTracer.xAxis[0], centroid[1]+ pointTracer.xAxis[1], centroid[2]+ pointTracer.xAxis[2]);
            glEnd();


            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex3f(centroid[0], centroid[1], centroid[2]);
            glVertex3f(centroid[0] + pointTracer.yAxis[0], centroid[1]+ pointTracer.yAxis[1], centroid[2]+ pointTracer.yAxis[2]);
            glEnd();

            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINES);
            glVertex3f(centroid[0], centroid[1], centroid[2]);
            glVertex3f(centroid[0] + pointTracer.zAxis[0], centroid[1]+ pointTracer.zAxis[1], centroid[2]+ pointTracer.zAxis[2]);
            glEnd();

            /*

            //now i'm fitting a curve
            pointTracer.CreateCurve(sphere, centroid, 0);
            std::vector<Eigen::Vector3f> curvePoints = pointTracer.GetCurvePoints(sphere, centroid);

            //setting point size of points
            glPointSize(3);
            glColor3f(1.0f, 1.0f, 0.0f);
            glBegin(GL_POINTS);

            for(int i = 0; i < curvePoints.size(); i++){
                Eigen::Vector3f point = curvePoints[i];
                glVertex3f(point[0], point[1], point[2]);
            }
            glEnd();



            Eigen::Vector3f rayOrigin = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            Eigen::Vector3f rayDirection = centroid - rayOrigin;
            Eigen::Vector3f normalTemp = rayDirection;
            normalTemp.normalize();
            Eigen::Vector3f rayNormal = normalTemp;

            MyRay centroidRay(rayOrigin, rayDirection, rayNormal, 0, 0);

            std::vector<Eigen::Vector3f> result = pointTracer.GetRayPoints(sphere, centroidRay);
            log << "result size: " << result.size() << endl;
            //result -= Eigen::Vector3f(-0.5, -0.5, -0.5);

            glPointSize(8);
            glColor3f(1.0f, 0.25f, 0.25f);
            glBegin(GL_POINTS);

            for(int i = 0; i < result.size(); i++){
                Eigen::Vector3f point = result[i];
                glVertex3f(point[0], point[1], point[2]);
            }

            glEnd();

            */

            window.update(et.getElapsedSeconds());
            window.flush();
        }

    }



    /*
    //create the framebuffer
    //specify values in terms of yindex, xindex, colorcomponent

    FloatImageRGBA fim(WINDOW_WIDTH,WINDOW_HEIGHT);

    //makes the soaper log output all messages
    se::core::Log::setVerbose(true);
    se::sim::ElapsedTimer et;

    unsigned frames = 0;
    try
    {
        srand(time(0));

        std::vector<PointObject> objectList;

        //making a sphere
        Eigen::Vector3f col(1.0f, 0.0f, 0.0f);
        PointObject sphere(col, 0.0f, 0.0f, -12.0f, 4.0f);
        objectList.push_back(sphere);


        //creating the PointTracer
        PointTracer pointTracer(objectList, WINDOW_WIDTH, WINDOW_HEIGHT);


        //do raytracing and update your framebuffer data here

        for(int j = 0; j < WINDOW_HEIGHT; j++)
        {
#pragma omp parallel for
            for(int i = 0; i < WINDOW_WIDTH; i++)
            {
                MyRay ray = pointTracer.GetOriginRay(j, i);

                Color pixelColor = pointTracer.GetColor(ray);

                //cap all the color component to one in case they go over
                if(pixelColor.redComp > 1.0f){
                    pixelColor.redComp = 1.0f;
                }
                if(pixelColor.greenComp > 1.0f){
                    pixelColor.greenComp = 1.0f;
                }
                if(pixelColor.blueComp > 1.0f){
                    pixelColor.blueComp = 1.0f;
                }


                fim.write(i,j,Color4f(se::math::Vector3D(pixelColor.redComp,pixelColor.greenComp,pixelColor.blueComp)));
            }

        }

        //upload the framebuffer to gpu
        log << "rendered a frame" << endl;
        formats::LdrImage lim(fim);
        lim.write("out.tga");

    }
    */


    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }


    return 0;
}


