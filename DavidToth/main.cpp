//#include <GL/glfw.h>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GLFW/glfw3.h"
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <iostream>
#include <cstdlib>

/*##HOMEMADE##*/
#include "camera.h"

#define __MAPSIZE__ 25

using namespace std;

/*##TYPEDEF##*/
struct Voxel{
    Voxel() : density (50), material(0){}
    int density;    /*1..100*/
    int material;   /*id's will be given*/
    float x,y,z;
};

/*##GLOBAL VARIABLES HERE##*/
    int         width = 1366, height = 768;
    float       moving = 0;
    float*      dispArr;
    int         dispArrSize;
    int         frame = 0;
    int         i = 0, j = 0, k = 0;
    Voxel***    field;
    GLuint      texture;
    bool        running = true;

    cameraT cam(Vector3D(__MAPSIZE__-2,__MAPSIZE__-2,__MAPSIZE__-2),Vector3D(0,0,0));
    int             ghostX,ghostY;

    /*GL_N3F_V3F*/
    /*[Nx][Ny][Nz][Vx][Vy][Vz]*/
    float* vertices;

void initGame();
void initScene();

Vector3D calcNormal(Vector3D triangle[3]);

/*The coordinates, the nubered tetrahedrons and the description are in the blender file named  tetrahedrons.blend*/
void marchingTetrahedrons(Voxel v0,Voxel v1,Voxel v2,Voxel v3, float **verts, int size, int* current, int tetrahedron){
    /*to be precise: the verts array currently is: [vertices][coordinates] = [n][6]
      so the size represents the number of vertices*/
    /*first we find out how many lands are in this tetrahedron(max is 4)*/
    int count = 0;
    if(v0.material != 0)count++;
    if(v1.material != 0)count++;
    if(v2.material != 0)count++;
    if(v3.material != 0)count++;

    /*depending on the number we extract the cases*/
    /*cout << "v0.material == " << v0.material << endl;
    cout << "v1.material == " << v1.material << endl;
    cout << "v2.material == " << v2.material << endl;
    cout << "v3.material == " << v3.material << endl;
    cout << "count == " << count << endl;
    cout << "=================" << endl;*/
    switch(count){
        case 0:
        case 4:
        break;

        case 1:
            /*One voxel has material*/
            if(v0.material != 0){       /*03,02,01*/
                if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 || tetrahedron == 6){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }if(tetrahedron == 2 || tetrahedron == 5){
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v1.material != 0){     /*01,13,12*/
                if(tetrahedron == 2 ||tetrahedron == 5){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 1 ||tetrahedron == 3||tetrahedron == 4|| tetrahedron == 6){
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v2.material != 0){     /*02,12,23*/
                if(tetrahedron == 2 || tetrahedron == 5){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 3||tetrahedron == 1 ||tetrahedron == 4 ||tetrahedron == 6){
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v3.material != 0){     /*03,32,31*/
                if(tetrahedron == 2 ||tetrahedron == 5){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v1.x)/2;
                    verts[(*current)][4] = (v3.y+v1.y)/2;
                    verts[(*current)][5] = (v3.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 ||tetrahedron == 6){
                    verts[(*current)][3] = (v3.x+v1.x)/2;
                    verts[(*current)][4] = (v3.y+v1.y)/2;
                    verts[(*current)][5] = (v3.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }
        break;

        case 2:
            /*two voxels have material*/
            if(v0.material != 0 && v3.material != 0){       /*01,02,31;31,02,32*/
                if(tetrahedron == 2 ||tetrahedron == 5 ){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v1.x)/2;
                    verts[(*current)][4] = (v3.y+v1.y)/2;
                    verts[(*current)][5] = (v3.z+v1.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v0.x)/2;
                    verts[(*current)][4] = (v2.y+v0.y)/2;
                    verts[(*current)][5] = (v2.z+v0.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 4||tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 6){
                    verts[(*current)][3] = (v3.x+v1.x)/2;
                    verts[(*current)][4] = (v3.y+v1.y)/2;
                    verts[(*current)][5] = (v3.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v0.x)/2;
                    verts[(*current)][4] = (v2.y+v0.y)/2;
                    verts[(*current)][5] = (v2.z+v0.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v1.material != 0 && v2.material != 0){     /*13,32,02;02,01,13*/
                if(tetrahedron == 1 ||tetrahedron == 4 || tetrahedron == 6 || tetrahedron == 3){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 2||tetrahedron == 5){
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v2.x)/2;
                    verts[(*current)][4] = (v3.y+v2.y)/2;
                    verts[(*current)][5] = (v3.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v2.material != 0 && v3.material != 0){     /*03,02,13;13,02,12*/
                if(tetrahedron == 2 || tetrahedron == 5){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 ||tetrahedron == 6){
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v0.material != 0 && v1.material != 0){     /*03,02,13;13,02,12*/
                if(tetrahedron == 3 ||tetrahedron == 6 || tetrahedron == 1 || tetrahedron == 4){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 2 || tetrahedron == 5 ){
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v1.material != 0 && v3.material != 0){     /*01,12,32;32,30,01*/
                if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 || tetrahedron == 6){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v0.x)/2;
                    verts[(*current)][4] = (v3.y+v0.y)/2;
                    verts[(*current)][5] = (v3.z+v0.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 2 ||tetrahedron == 5){
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v3.x+v0.x)/2;
                    verts[(*current)][4] = (v3.y+v0.y)/2;
                    verts[(*current)][5] = (v3.z+v0.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v0.material != 0 && v2.material != 0){     /*01,03,32;32,12,01*/
                if(tetrahedron == 1  ||tetrahedron == 3 ||tetrahedron == 4||tetrahedron == 6){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 5||tetrahedron == 2){
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;

                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }
        break;

        case 3:
            /*three voxels have material*/
            if(v0.material == 0){       /*03,01,02*/
                if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 ||tetrahedron == 6 ){
                    /*correct tetrahedrons for this winding: 1,3,4,6*/
                    /*wrong tetrahedrons for this winding: 2,5*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 2 || tetrahedron == 5){
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v1.material == 0){     /*10,12,13*/
                if(tetrahedron == 2 ||tetrahedron == 5 ){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 ||tetrahedron == 6 ){
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v2.x)/2;
                    verts[(*current)][4] = (v1.y+v2.y)/2;
                    verts[(*current)][5] = (v1.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v1.x)/2;
                    verts[(*current)][4] = (v0.y+v1.y)/2;
                    verts[(*current)][5] = (v0.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v2.material == 0){     /*20,23,21*/
                if(tetrahedron == 2 || tetrahedron == 5 ){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v1.x)/2;
                    verts[(*current)][4] = (v2.y+v1.y)/2;
                    verts[(*current)][5] = (v2.z+v1.z)/2;
                    (*current) = (*current)+1;/**/
                } else if(tetrahedron == 1 ||tetrahedron == 3 ||tetrahedron == 4 ||tetrahedron == 6){
                    verts[(*current)][3] = (v2.x+v1.x)/2;
                    verts[(*current)][4] = (v2.y+v1.y)/2;
                    verts[(*current)][5] = (v2.z+v1.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v2.x)/2;
                    verts[(*current)][4] = (v0.y+v2.y)/2;
                    verts[(*current)][5] = (v0.z+v2.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }else if(v3.material == 0){     /*30,31,32*/
                if(tetrahedron == 2 ||tetrahedron == 5  ){
                    /*correct tetrahedrons for this winding: 2,5*/
                    /*wrong tetrahedrons for this winding: 1,3,4,6*/
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }else if(tetrahedron == 3 ||tetrahedron == 1 ||tetrahedron == 4 ||tetrahedron == 6){
                    verts[(*current)][3] = (v2.x+v3.x)/2;
                    verts[(*current)][4] = (v2.y+v3.y)/2;
                    verts[(*current)][5] = (v2.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v1.x+v3.x)/2;
                    verts[(*current)][4] = (v1.y+v3.y)/2;
                    verts[(*current)][5] = (v1.z+v3.z)/2;
                    (*current) = (*current)+1;
                    verts[(*current)][3] = (v0.x+v3.x)/2;
                    verts[(*current)][4] = (v0.y+v3.y)/2;
                    verts[(*current)][5] = (v0.z+v3.z)/2;
                    (*current) = (*current)+1;/**/
                }
            }
        break;
    }
}

float** generateVertices(Voxel*** inField, int* size){
    /*size means how many vertices are going to be in the array*/
    /*testdata will be just squares, so there will be n*3*2 vertices */
    int count = 0;
    int current = 0;
    float** verts;
    float** tverts;

    /*at maximum we have (roughly) __MAPSIZE__*__MAPSIZE__*__MAPSIZE__ cubes, all of which can use
      (roughly) 6 tetrahedrons, and every tetrahedrons maximum triangle count is 2. every tringle has 3 vertices.
      Every vertex will have 6 coordinates(3 space coordinates, 3 normal vector coordinates)
      Considering all this, an estimated maximum we will have __MAPSIZE__*__MAPSIZE__*__MAPSIZE__*6*2*3*6.
      That's 27000000. Holy shit.
      Here we will use [__MAPSIZE__*__MAPSIZE__*__MAPSIZE__*6*2*3] elements as vertices, and [6] for coordinates
    */

    /*reserve memory for the array*/
    count = __MAPSIZE__*__MAPSIZE__*__MAPSIZE__*6*2*3;
    tverts = new float*[count];
    for(i=0;i<count;i++)tverts[i] = new float[6];

    /*go through the field cube to cube. A cube will have an edge of 1.*/
    for(i=0;i<__MAPSIZE__-1;i++)for(j=0;j<__MAPSIZE__-1;j++)for(k=0;k<__MAPSIZE__-1;k++){
        /*and we call the tetrahedrons to the 8 vertices of the cube*/
        /*!Thank you paul Burke! (work needed in the booklet)
              PolygoniseTri(grid,iso,triangles,0,2,3,7);
              PolygoniseTri(grid,iso,triangles,0,2,6,7);
              PolygoniseTri(grid,iso,triangles,0,4,6,7);

              PolygoniseTri(grid,iso,triangles,0,6,1,2);
              PolygoniseTri(grid,iso,triangles,0,6,1,4);
              PolygoniseTri(grid,iso,triangles,5,6,1,4);
        */
        /*1*/marchingTetrahedrons(inField[i][j][k+1],inField[i+1][j][k],inField[i][j][k],inField[i][j+1][k],tverts,count,&current,1);/**/
        /*2*/marchingTetrahedrons(inField[i][j][k+1],inField[i+1][j][k],inField[i+1][j+1][k],inField[i][j+1][k],tverts,count,&current,2);/**/
        /*3*/marchingTetrahedrons(inField[i][j][k+1],inField[i][j+1][k+1],inField[i+1][j+1][k],inField[i][j+1][k],tverts,count,&current,3);/**/
        /*4*/marchingTetrahedrons(inField[i][j][k+1],inField[i+1][j+1][k],inField[i+1][j][k+1],inField[i+1][j][k],tverts,count,&current,4);/**/
        /*5*/marchingTetrahedrons(inField[i][j][k+1],inField[i+1][j+1][k],inField[i+1][j][k+1],inField[i][j+1][k+1],tverts,count,&current,5);/**/
        /*6*/marchingTetrahedrons(inField[i+1][j+1][k+1],inField[i+1][j+1][k],inField[i+1][j][k+1],inField[i][j+1][k+1],tverts,count,&current,6);/**/
    }

    /*After the algorithm, I copy every triangle to the appropriate sized result array(verts)*/
    verts = new float*[current];
    for(i=0;i<current;i++)verts[i] = new float[6];

    for(i=0;i<current;i++){
        verts[i][3] = tverts[i][3];
        verts[i][4] = tverts[i][4];
        verts[i][5] = tverts[i][5];
    }

    delete[]tverts;

    /*and give the algorithm the number of the vertices*/
    (*size) = current;

    return verts;
}

void normalizeVertices(float** inverts, int size){
 Vector3D   tempV[3];
 Vector3D   tNormal;
    for(i=0; i<size; i+=3){

        tempV[0] = Vector3D(inverts[i][3],inverts[i][4],inverts[i][5]);
        tempV[1] = Vector3D(inverts[i+1][3],inverts[i+1][4],inverts[i+1][5]);
        tempV[2] = Vector3D(inverts[i+2][3],inverts[i+2][4],inverts[i+2][5]);
        tNormal = calcNormal(tempV);

        inverts[i][0] = tNormal.x;
        inverts[i][1] = tNormal.y;
        inverts[i][2] = tNormal.z;

        inverts[i+1][0] = tNormal.x;
        inverts[i+1][1] = tNormal.y;
        inverts[i+1][2] = tNormal.z;

        inverts[i+2][0] = tNormal.x;
        inverts[i+2][1] = tNormal.y;
        inverts[i+2][2] = tNormal.z;
    }
}

float* arrayizeVertices(float** inverts, int size){
 float* verts;
 int count = 0;

    verts = new float[size*6];

    for(i=0; i<size;i++){
        verts[count] = inverts[i][0];
        verts[count+1] = inverts[i][1];
        verts[count+2] = inverts[i][2];
        verts[count+3] = inverts[i][3];
        verts[count+4] = inverts[i][4];
        verts[count+5] = inverts[i][5];
        count += 6;
    }
    return verts;
}

void resizeF(GLFWwindow *window, int w, int h ){
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,ar,1,100);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*##KEY EVENTS HERE##*/
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
			running = false;    /*kilépés*/
        }
		else if(key == 'W') {
            moving =0.1;
        }
		else if(key == 'S') {
            moving = -0.1;
        }
		else if(key == GLFW_KEY_SPACE) {
            moving = moving * 5;
        }
		else if(key == GLFW_KEY_TAB) {
            initGame();
            /*make an array from the vertices,
             which was added normal vectors from
             the generated vertices from the field
                --WE NEED TO GO DERPER--
            */
            delete[] dispArr;
            float** tempPoint = generateVertices(field,&dispArrSize);
            normalizeVertices( tempPoint,dispArrSize);
            dispArr = arrayizeVertices(tempPoint,dispArrSize);
        }
    }if(action == GLFW_RELEASE){
        /*SOON*/
        if(key == 'W'){
            moving = 0;
        }else if(key == 'S'){
            moving = 0;
        }else if(key == GLFW_KEY_SPACE){
            moving = moving / 5;
        }
    }
}


void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    int Y(width-x),X(height-y);

    cam.PushAngleVect(Vector3D((float)(X-ghostX),(float)(Y-ghostY),0));
    ghostX = X; ghostY = Y;
}

void updateGameF(){
    frame++;
    cam.moveForward(moving);
}

Vector3D calcNormal(Vector3D triangle[3]){
 Vector3D U,V,N;

    Vector3D normVec;

    U = triangle[1] - triangle[0];
    V = triangle[2] - triangle[0];

    N.x = (U.y*V.z)-(U.z*V.y);
    N.y = (U.z*V.x)-(U.x*V.z);
    N.z = (U.x*V.y)-(U.y*V.x);

    normVec.x = N.x;
    normVec.y = N.y;
    normVec.z = N.z;
    return normVec;

}


void initScene()
{

    /*--|--hide the cursor?*/
    /*glfwDisable(GLFW_MOUSE_CURSOR);*/
    /*--joystick*/
    /*##SOON##*/

    /*Graphic settings*/
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glPointSize(10);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    /*Lightning*/
    static const GLfloat afAmbientWhite [] = {0.25, 0.25, 0.25, 1.00};
    static const GLfloat afAmbientRed   [] = {0.25, 0.00, 0.00, 1.00};
    static const GLfloat afAmbientGreen [] = {0.00, 0.25, 0.00, 1.00};
    static const GLfloat afAmbientBlue  [] = {0.00, 0.00, 0.25, 1.00};
    static const GLfloat afDiffuseWhite [] = {0.75, 0.75, 0.75, 1.00};
    static const GLfloat afDiffuseRed   [] = {0.75, 0.00, 0.00, 1.00};
    static const GLfloat afDiffuseGreen [] = {0.00, 0.75, 0.00, 1.00};
    static const GLfloat afDiffuseBlue  [] = {0.00, 0.00, 0.75, 1.00};
    static const GLfloat afSpecularWhite[] = {1.00, 1.00, 1.00, 1.00};
    static const GLfloat afSpecularRed  [] = {1.00, 0.25, 0.25, 1.00};
    static const GLfloat afSpecularGreen[] = {0.25, 1.00, 0.25, 1.00};
    static const GLfloat afSpecularBlue [] = {0.25, 0.25, 1.00, 1.00};
        GLfloat afPropertiesAmbient [] = {0.50, 0.50, 0.50, 1.00};
        GLfloat afPropertiesDiffuse [] = {0.75, 0.75, 0.75, 1.00};
        GLfloat afPropertiesSpecular[] = {1.00, 1.00, 1.00, 1.00};
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glLightfv( GL_LIGHT0, GL_AMBIENT,  afPropertiesAmbient);
        glLightfv( GL_LIGHT0, GL_DIFFUSE,  afPropertiesDiffuse);
        glLightfv( GL_LIGHT0, GL_SPECULAR, afPropertiesSpecular);
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

        glEnable( GL_LIGHT0 );

        glMaterialfv(GL_BACK,  GL_AMBIENT,   afAmbientGreen);
        glMaterialfv(GL_BACK,  GL_DIFFUSE,   afDiffuseWhite);
        glMaterialfv(GL_FRONT, GL_AMBIENT,   afAmbientBlue);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   afDiffuseBlue);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  afSpecularWhite);
        glMaterialf( GL_FRONT, GL_SHININESS, 25.0);
}

void initGame(){
    /*##INITIALIZE GAME DATA HERE##*/
    /*init field*/
    field = new Voxel**[__MAPSIZE__];
    for(i=0; i<__MAPSIZE__; i++){
        field[i] = new Voxel*[__MAPSIZE__];
        for(j=0; j<__MAPSIZE__; j++){
            field[i][j] = new Voxel[__MAPSIZE__];
            for(k=0; k<__MAPSIZE__;k++){
                field[i][j][k].x = i;
                field[i][j][k].y = j;
                field[i][j][k].z = k;
            }
        }
    }
    /*Random
    for(i=0;i<__MAPSIZE__;i++)for(j=0;j<__MAPSIZE__;j++)for(k=0;k<__MAPSIZE__;k++){
        if(rand()%50 == 1)field[i][j][k].material = 1;
    }
    */
    /*put a big square to the bottom center*/
    int start = (__MAPSIZE__/2)-(__MAPSIZE__/8),
        end = (__MAPSIZE__/2)+(__MAPSIZE__/8);
    for(i=start;i<end;i++)for(j=0;j<end;j++)for(k=start;k<end;k++){
        if(rand()%2 == 1)field[i][j][k].material = 1;
    }
    /*test cube
    field[__MAPSIZE__-3][__MAPSIZE__-3][__MAPSIZE__-3].material = 1;
    field[__MAPSIZE__-4][__MAPSIZE__-3][__MAPSIZE__-3].material = 1;
    field[__MAPSIZE__-3][__MAPSIZE__-4][__MAPSIZE__-3].material = 1;
    field[__MAPSIZE__-4][__MAPSIZE__-4][__MAPSIZE__-3].material = 1;

    field[__MAPSIZE__-3][__MAPSIZE__-3][__MAPSIZE__-4].material = 1;
    field[__MAPSIZE__-4][__MAPSIZE__-3][__MAPSIZE__-4].material = 1;
    field[__MAPSIZE__-3][__MAPSIZE__-4][__MAPSIZE__-4].material = 1;
    field[__MAPSIZE__-4][__MAPSIZE__-4][__MAPSIZE__-4].material = 1;
    */
    /*put a plate in the center*/
    j= j/2;
    start = (__MAPSIZE__/2)-(__MAPSIZE__/4);
    end = (__MAPSIZE__/2)+(__MAPSIZE__/4);
    for(i=start;i<end;i++)for(k=start;k<end;k++){
        if(rand()%2 == 1)field[i][j][k].material = 1;
    }
}

GLFWwindow *window = nullptr;

int main()
{
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, "Marching tetrahedrons test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, resizeF);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, width / (double)height, -1.0, 1000);

    initScene();
    initGame();

    float** tempPoint = generateVertices(field,&dispArrSize);
    normalizeVertices( tempPoint,dispArrSize);
    dispArr = arrayizeVertices(tempPoint,dispArrSize);
    while(!glfwWindowShouldClose(window) && running){
        updateGameF();

        glClearColor( 0.5f, 0.5f, 0.9f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_LIGHTING);
        glPushMatrix();
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        gluPerspective(60,(width/height),1,100);

        /*##CAMERA HANDLING HERE*/
        //gluLookAt(0,0,1,0,0,0,0,1,0);
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        cam.render();

        /*##RENDERING HERE##*/
        glPushMatrix();
            /*rendering the field*/
            glTranslatef(-0.5f,-0.5f,-0.5f);    /*because the field's objects have integer coordinates for now*/
            /*x,z - red*/
            /*draw the floor and ceiling*/
            glColor3f(1,0,0);
            glBegin(GL_LINES);
                for(i=0;i<=__MAPSIZE__;i++){
                    glVertex3f(i,0,0);
                    glVertex3f(i,0,__MAPSIZE__);

                    glVertex3f(i,__MAPSIZE__,0);
                    glVertex3f(i,__MAPSIZE__,__MAPSIZE__);

                    glVertex3f(0,0,i);
                    glVertex3f(__MAPSIZE__,0,i);

                    glVertex3f(0,__MAPSIZE__,i);
                    glVertex3f(__MAPSIZE__,__MAPSIZE__,i);
                }
            glEnd();
            /*y,x - green*/
            /*I want to see walls*/
            glColor3f(0,1,0);
            glBegin(GL_LINES);
                for(i=0;i<=__MAPSIZE__;i++){
                    glVertex3f(0,i,0);
                    glVertex3f(__MAPSIZE__,i,0);

                    glVertex3f(0,i,__MAPSIZE__);
                    glVertex3f(__MAPSIZE__,i,__MAPSIZE__);

                    glVertex3f(i,0,0);
                    glVertex3f(i,__MAPSIZE__,0);

                    glVertex3f(i,0,__MAPSIZE__);
                    glVertex3f(i,__MAPSIZE__,__MAPSIZE__);
                }
            glEnd();
            /*z,y - blue*/
            /*I want to see walls*/
            glColor3f(0,0,1);
            glBegin(GL_LINES);
                for(i=0;i<=__MAPSIZE__;i++){
                    glVertex3f(0,0,i);
                    glVertex3f(0,__MAPSIZE__,i);

                    glVertex3f(__MAPSIZE__,0,i);
                    glVertex3f(__MAPSIZE__,__MAPSIZE__,i);

                    glVertex3f(0,i,0);
                    glVertex3f(0,i,__MAPSIZE__);

                    glVertex3f(__MAPSIZE__,i,0);
                    glVertex3f(__MAPSIZE__,i,__MAPSIZE__);
                }
            glEnd();
        glPopMatrix();
        glEnable(GL_LIGHTING);
        /*glDisable(GL_LIGHTING);
        for(i=0;i<__MAPSIZE__;i++)for(j=0;j<__MAPSIZE__;j++)for(k=0;k<__MAPSIZE__;k++){
            glPushMatrix();
                glTranslatef(i,j,k);
                if(field[i][j][k].material == 0) glColor3f(1,1,1);
                else glColor3f(0,0,0);
                glBegin(GL_POINTS);
                    glVertex3f(0,0,0);
                glEnd();
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);*/

        /*Now draw the field*/
        glEnableClientState(GL_VERTEX_ARRAY);
            glInterleavedArrays(GL_N3F_V3F,0,dispArr);
            glDrawArrays(GL_TRIANGLES,0,dispArrSize);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
        glfwSwapBuffers(window);
		glfwPollEvents();
    }
    glfwTerminate();
    /*##CLEAR GARBAGE##*/
    return 0;
}
