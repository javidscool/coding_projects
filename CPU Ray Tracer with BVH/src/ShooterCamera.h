#ifndef VIRTUOSO_SHOOTER_CAMERA
#define VIRTUOSO_SHOOTER_CAMERA
#include <se/math/RollingSum.h>
#include<boost/bind.hpp> //needed for non-trivial boost function usages.
#include <se/graphics/DepthCamera.h>
#include <se/sim/ui/PCInputBinding.h>
#include <se/graphics/gl/PCGLWindow.h>
#include "transformations.h"


using namespace se::sim::ui;

class ShooterCamera:/* public se::graphics::DepthCamera,*/public se::sim::ui::PCInputBinding
{

public:

    se::math::RollingSum<3> smoothX;
    se::math::RollingSum<3> smoothY;

    double oldx;
    double oldy;
    bool ignore;
    bool running;

    se::graphics::gl::PCGLWindow& window;


    double phi;
    double theta;
    se::math::Vector3D position;

    void escape()
    {
        running = false;
        //cout << "ESCAPE" << endl;
    }


    ShooterCamera(se::graphics::gl::PCGLWindow& win);


    Eigen::Matrix4f getMatrix()
    {

        Eigen::Matrix4f vm = Eigen::Matrix4f::Identity();
        vm = vm * rotateX(phi);
        vm  = vm * rotateY(theta);

        vm = vm * translation(position.x, position.y, position.z);

        return vm;
    }


    void sPressed()
    {
        double r = .1;
        double pi2 = 3.14/2.0;
        se::math::Vector3D inc(r * cos(theta-pi2),0.0,r*sin(theta-pi2));



        position = (position + inc);//se::math::Vector3D(0,0,.01));


    }

    void wPressed()
    {
        double r = .1;
        double pi2 = 3.14/2.0;
        se::math::Vector3D inc(-r * cos(theta-pi2),0.0,-r*sin(theta-pi2));



        position = (position + inc);//se::math::Vector3D(0,0,.01));


    }



    void aPressed()
    {
        double r = .1;
        double pi2 = 3.14/2.0;
        se::math::Vector3D inc(r * cos(theta),0.0,r*sin(theta));



        position = (position + inc);//se::math::Vector3D(0,0,.01));


    }



    void dPressed()
    {
        double r = .1;
        double pi2 = 3.14/2.0;
        se::math::Vector3D inc(-r * cos(theta),0.0,-r*sin(theta));



        position = (position + inc);//se::math::Vector3D(0,0,.01));

    }



    void mouseMoved(const Pointer& ptr);


};


#endif
