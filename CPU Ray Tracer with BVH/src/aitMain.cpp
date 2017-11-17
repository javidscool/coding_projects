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

#include "RayTracer.h"
#include "Misc.h"
#include "BVH.h"
#include <cfloat>
#include <algorithm>


using namespace std;
using namespace se::graphics::gl;
using namespace se::graphics::image;


//you define this.  controls window dimensions, creates framebuffer for ray tracing
const unsigned int WINDOW_WIDTH = 512;
const unsigned int WINDOW_HEIGHT = 512;

//typedef GLubyte pixel[3];


int main(int argc, char** argv)
{


    //create the framebuffer
    //specify values in terms of yindex, xindex, colorcomponent

    FloatImageRGBA fim(WINDOW_WIDTH,WINDOW_HEIGHT);

    //makes the soaper log output all messages
    se::core::Log::setVerbose(true);
    se::sim::ElapsedTimer et;

    unsigned frames = 0;
    try
    {
        //filename of the lego man mesh and can mesh
        std::string infilename1 = "lego.vbo";
        std::string infilename2 = "can0.vbo";

        std::string textfilename1 = "Robot.png";
        std::string textfilename2 = "can1dif.jpg";

        //load in our meshes
        std::ifstream intest(infilename1.c_str(), std::ios::in | std::ios::binary);
        Mesh cmeshLego(intest);
        intest.close();

        std::ifstream intest2(infilename2.c_str(), std::ios::in | std::ios::binary);
        Mesh cmeshCan(intest2);
        intest2.close();

        //get the list of triangles from the mesh objects
        std::vector<Triangle> triangleListLego = cmeshLego.toTriangleList();
        std::vector<Triangle> triangleListCan = cmeshCan.toTriangleList();


        std::vector<Object> objectList;
        std::vector<BVH> BVHList;



        //matrix to take lego man from object space to world space
        Eigen::Matrix4f transLego = translation(0.0f, 0.0f, -12.0f);
        Eigen::Matrix4f scaleLego = scale(1.4f, 1.4f, 1.4f);

        //modelview matrix is the camera matrix times the object matrix
        //different for each object
        Eigen::Matrix4f modelviewLego = transLego * scaleLego;

        //the normal matrix of an object is the inverse transpose of the upper left of the
        //modelview matrix
        Eigen::Matrix3f normalmatrixObjLego = (modelviewLego).topLeftCorner<3,3>().inverse().transpose();

        //making the lego man
        Object legoMan(triangleListLego, modelviewLego, normalmatrixObjLego, 2.50f, 0.8f, 0.0f, textfilename1);
        objectList.push_back(legoMan);

        //creating BVH for the lego
        log << "building BVH for lego" << endl;
        BVH legoBVH(legoMan.GetTriangles());
        log << "lego BVH done" << endl;
        BVHList.push_back(legoBVH);


        //matrix to take can from object space to world space
        Eigen::Matrix4f transCan = translation(0.0f, -1.0f, -4.0f);
        Eigen::Matrix4f scaleCan = scale(0.45f, 0.45f, 0.45f);
        //Eigen::Matrix4f transCan = translation(5.0f, -3.0f, -16.0f);
        //Eigen::Matrix4f scaleCan = scale(1.5f, 1.5f, 1.5f);

        //modelview matrix is the camera matrix times the object matrix
        //different for each object
        Eigen::Matrix4f modelviewCan = transCan * scaleCan;

        //the normal matrix of an object is the inverse transpose of the upper left of the
        //modelview matrix
        Eigen::Matrix3f normalmatrixObjCan = (modelviewCan).topLeftCorner<3,3>().inverse().transpose();

        //making the can
        Object can(triangleListCan, modelviewCan, normalmatrixObjCan, 1.33f, 0.0f, 1.0f, textfilename2);
        objectList.push_back(can);

        //creating BVH for the can
        log << "building BVH for can" << endl;
        BVH canBVH(can.GetTriangles());
        log << "can BVH done" << endl;
        BVHList.push_back(canBVH);




        //creating the RayTracer
        RayTracer rayTracer(objectList, BVHList, WINDOW_WIDTH, WINDOW_HEIGHT);


        //do raytracing and update your framebuffer data here
        for(int j = 0; j < WINDOW_HEIGHT; j++)
        {
            for(int i = 0; i < WINDOW_WIDTH; i++)
            {
                MyRay ray = rayTracer.GetOriginRay(j, i);

                //NOT using supersampling
                Color pixelColor = rayTracer.GetColor(ray);

                //using supersampling
                //Color pixelColor = rayTracer.AdaptiveSuperSamp(ray, j, i);

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
        rayTracer.fout.close();

    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

