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
#include "Lightmap.h"
#include "Misc.h"
#include <cfloat>
#include <algorithm>


using namespace std;
using namespace se::graphics::gl;
using namespace se::graphics::image;



int main(int argc, char** argv)
{

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
        Object legoMan(triangleListLego, modelviewLego, normalmatrixObjLego, 2.50f, 0.0f, 0.0f, textfilename1);

        //leave out texture information for robot
        //legoMan.ToggleTexture();

        objectList.push_back(legoMan);



        //matrix to take can from object space to world space
        Eigen::Matrix4f transCan = translation(5.0f, -3.0f, -16.0f);
        Eigen::Matrix4f scaleCan = scale(1.5f, 1.5f, 1.5f);

        //modelview matrix is the camera matrix times the object matrix
        //different for each object
        Eigen::Matrix4f modelviewCan = transCan * scaleCan;

        //the normal matrix of an object is the inverse transpose of the upper left of the
        //modelview matrix
        Eigen::Matrix3f normalmatrixObjCan = (modelviewCan).topLeftCorner<3,3>().inverse().transpose();

        //making the can
        Object Can(triangleListCan, modelviewCan, normalmatrixObjCan, 1.33f, 0.0f, 0.0f, textfilename2);

        //leave out texture information for the can
        //Can.ToggleTexture();

        objectList.push_back(Can);



        //creating the lightmap generator
        Lightmap lightMap(objectList);


        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;


        //this loop here goes through the lightmap's objectList and makes one lightmap per object
        for(int obj = 0; obj < lightMap.objectList.size(); obj++){

            //getting the texture height and width for this object
            WINDOW_WIDTH = lightMap.objectList[obj].texture->width;
            WINDOW_HEIGHT = lightMap.objectList[obj].texture->height;

            //create the framebuffer
            FloatImageRGBA fim(WINDOW_WIDTH, WINDOW_HEIGHT);

            //do lightmapping and update your framebuffer data here
            for(int j = 0; j < WINDOW_HEIGHT; j++)
            {
                for(int i = 0; i < WINDOW_WIDTH; i++)
                {
                    float v = ((float)j/(float)WINDOW_HEIGHT);
                    float u = ((float)i/(float)WINDOW_WIDTH);

                    //converting the i and j to 0-1 range as I pass them in
                    Color pixelColor = lightMap.GetColor(lightMap.objectList[obj], v, u);


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
            log << "made a lightmap for an object" << endl;
            formats::LdrImage lim(fim);

            stringstream ss;
            ss << (obj + 1);

            std::string outfilename1 = "object" + ss.str() + ".tga";

            lim.write(outfilename1);

        }



    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

