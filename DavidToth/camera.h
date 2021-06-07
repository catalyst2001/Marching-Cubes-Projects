#define M_PI       3.14159265358979323846   // pi

//math
#include <math.h>

//##HOMEMADE##
#include "Vector3D.h"


float dgrTorad(float dgr){
	return (float)dgr*(M_PI/180);
}

float radTodgr(float rad){
	return (float)rad*(180/M_PI);
}

class cameraT{
    public:
     Vector3D eye, focus, angle;
     bool firstPerson;
        cameraT(Vector3D eye, Vector3D focus);

        void doTheMath();                               //a kamera szemei pozíciójának kiszámolása
        void PushAngleVect(Vector3D angle);             //Szögvektor hozzáadása
        void render();                                  //a megjelenítési függvények
        void moveForward(float dist);                   //menj elõre dist unitot

        ~cameraT();
};

void cameraT::render(){
 Vector3D tempV;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(firstPerson){    //eye,center,up
        tempV = eye + focus;
        gluLookAt(eye.x,eye.y,eye.z,tempV.x,tempV.y,tempV.z,0,1,0);
    }else{
        tempV = eye - focus;
        gluLookAt(tempV.x,tempV.y,tempV.z,eye.x,eye.y,eye.z,0,1,0);
    }
}

void cameraT::moveForward(float dist){
    eye = eye + focus*dist;
}

void cameraT::doTheMath(){

    //Thank you CodeColony.de!
    	Vector3D Step1, Step2;
        //Rotate around Y-axis:
        Step1.x = cos( dgrTorad(angle.y + 90.0));
        Step1.z = -sin( dgrTorad(angle.y + 90.0));
        //Rotate around X-axis:
        double cosX = cos (dgrTorad(angle.x));
        Step2.x = Step1.x * cosX;
        Step2.z = Step1.z * cosX;
        Step2.y = sin(dgrTorad(angle.x));
        //Rotation around Z-axis not yet implemented, so:
        focus = Step2;    //WHY DOES THIS WORK?!?!?!?!


}

void cameraT::PushAngleVect(Vector3D angle){
 bool x(angle.x+this->angle.x < 90 && angle.x+this->angle.x > -90),
    y(angle.y+this->angle.y < 90 && angle.y+this->angle.y > -90),
    z(angle.z+this->angle.z < 90 && angle.z+this->angle.z > -90);
    if(x){
        this->angle = this->angle + angle;
        doTheMath();
    }
}

cameraT::~cameraT(){
    //SOON
}

cameraT::cameraT(Vector3D eye, Vector3D focus){
    this->eye = eye;
    this->focus = focus;
    firstPerson = true;
    this->PushAngleVect(Vector3D(0,0,0));
}
