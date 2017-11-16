#include "ShooterCamera.h"


void ShooterCamera::mouseMoved(const Pointer& ptr)
{

    if(ignore)
    {
        ignore = false;
        return;
    }

    double xinc2 = 0;
    double yinc2 = 0;
    double rate = .0050;
    xinc2 = rate * (ptr.x - 128);
    yinc2 = rate * (ptr.y - 128);

    smoothX.push(xinc2);
    smoothY.push(yinc2);

    double xinc = smoothX.getMean();
    double yinc = smoothY.getMean();

    oldx = ptr.x;
    oldy = ptr.y;

//std::cout<<(phi/3.14)<<std::endl;

    if(phi + yinc < -3.14159 / 2.0)
    {
//phi += yinc;
        yinc=0;//(- 3.14159/ 2.0) - phi;
        phi = -3.14159 / 2.0;
    }
    else if(phi + yinc > 3.14159 / 2.0)
    {
//phi += yinc;
        yinc=0;//(- 3.14159/ 2.0) - phi;
        phi = 3.14159 / 2.0;
    }
    else
    {
        phi += yinc;
    }

    theta += xinc;
//setRotationMatrix(Eigen::Matrix3d::Identity());
    //cout << "MOUSE MOVED" << ptr.getNormalized() << endl;
//rotate(theta,0.0);
//rotate(0.0, -phi);




    se::math::Vector3D loc(128, 128, 0);

    ignore = true;



    window.setPointerLocation(loc);


}



ShooterCamera::ShooterCamera(se::graphics::gl::PCGLWindow& win):window(win), phi(0.0), theta(0.0), position(se::math::Vector3D(0,0,0))
{




    ignore = true;

    oldx = 128.0;
    oldy = 128.0;

    running = true;

    se::math::Vector3D loc(128, 128, 0);

//    perspective(3.14159/4.0, 4.0/3.0);

    // depth(.01, 100);

    position = (se::math::Vector3D(0,0,0));

    //bindButtonDown(PCInputBinding::MOUSE_LEFT, mouseleft);
    // bindButtonDown(PCInputBinding::MOUSE_RIGHT, mouseright);
    //binding.bindButtonDown(PCInputBinding::MOUSE_WHEEL_UP, wheelUp);
    //binding.bindButtonDown(PCInputBinding::MOUSE_WHEEL_DOWN, wheelDown);
//    bindButtonDown(PCInputBinding::ESCAPE, ShooterCamera::escape);
    // bindButtonDown('a', handleA);
    //  bindButtonUp('a', handleAUp);
    //  bindMouseMove(mouseMoved);
    bindButtonDown('s', boost::bind(&ShooterCamera::sPressed,this));
    bindButtonDown('w', boost::bind(&ShooterCamera::wPressed,this));
    bindButtonDown('a', boost::bind(&ShooterCamera::aPressed,this));
    bindButtonDown('d', boost::bind(&ShooterCamera::dPressed,this));
    bindButtonDown(PCInputBinding::ESCAPE, boost::bind(&ShooterCamera::escape,this));
    bindMouseMove(boost::bind(&ShooterCamera::mouseMoved,this,_1));

    win.setPointerLocation(loc);


}
