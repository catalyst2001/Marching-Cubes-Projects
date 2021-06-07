#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "paulslib.h"
#include "bitmaplib.h"
#include "opengllib.h"
#include "win32_dialogs.h"
#include "glut.h"
#pragma comment(lib, "glut32.lib")

//typedef struct {
//	double x, y, z;
//} XYZ;
//typedef struct {
//	XYZ p[3];
//} TRIANGLE;
//typedef struct {
//	XYZ p[8];
//	double val[8];
//} GRIDCELL;

//XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2)
//{
//	double mu;
//	XYZ p;
//	if (fabs(isolevel - valp1) < 0.00001f)
//		return(p1);
//	if (fabs(isolevel - valp2) < 0.00001f)
//		return(p2);
//	if (fabs(valp1 - valp2) < 0.00001f)
//		return(p1);
//	mu = (isolevel - valp1) / (valp2 - valp1);
//	p.x = p1.x + mu * (p2.x - p1.x);
//	p.y = p1.y + mu * (p2.y - p1.y);
//	p.z = p1.z + mu * (p2.z - p1.z);
//	return(p);
//}

/*
   Polygonise a tetrahedron given its vertices within a cube
   This is an alternative algorithm to polygonisegrid.
   It results in a smoother surface but more triangular facets.

					  + 0
					 /|\
					/ | \
				   /  |  \
				  /   |   \
				 /    |    \
				/     |     \
			   +-------------+ 1
			  3 \     |     /
				 \    |    /
				  \   |   /
				   \  |  /
					\ | /
					 \|/
					  + 2

   It's main purpose is still to polygonise a gridded dataset and
   would normally be called 6 times, one for each tetrahedron making
   up the grid cell.
   Given the grid labelling as in PolygniseGrid one would call
	  PolygoniseTri(grid,iso,triangles,0,2,3,7);
	  PolygoniseTri(grid,iso,triangles,0,2,6,7);
	  PolygoniseTri(grid,iso,triangles,0,4,6,7);
	  PolygoniseTri(grid,iso,triangles,0,6,1,2);
	  PolygoniseTri(grid,iso,triangles,0,6,1,4);
	  PolygoniseTri(grid,iso,triangles,5,6,1,4);
*/
//int PolygoniseTri(GRIDCELL g, double iso,
//	TRIANGLE *tri, int v0, int v1, int v2, int v3)
//{
//	int ntri = 0;
//	int triindex;
//
//	/*
//	   Determine which of the 16 cases we have given which vertices
//	   are above or below the isosurface
//	*/
//	triindex = 0;
//	if (g.val[v0] < iso) triindex |= 1;
//	if (g.val[v1] < iso) triindex |= 2;
//	if (g.val[v2] < iso) triindex |= 4;
//	if (g.val[v3] < iso) triindex |= 8;
//
//	/* Form the vertices of the triangles for each case */
//	switch (triindex) {
//	case 0x00:
//	case 0x0F:
//		break;
//	case 0x0E:
//	case 0x01:
//		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
//		ntri++;
//		break;
//	case 0x0D:
//	case 0x02:
//		tri[0].p[0] = VertexInterp(iso, g.p[v1], g.p[v0], g.val[v1], g.val[v0]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
//		ntri++;
//		break;
//	case 0x0C:
//	case 0x03:
//		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
//		ntri++;
//		tri[1].p[0] = tri[0].p[2];
//		tri[1].p[1] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
//		tri[1].p[2] = tri[0].p[1];
//		ntri++;
//		break;
//	case 0x0B:
//	case 0x04:
//		tri[0].p[0] = VertexInterp(iso, g.p[v2], g.p[v0], g.val[v2], g.val[v0]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v2], g.p[v1], g.val[v2], g.val[v1]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
//		ntri++;
//		break;
//	case 0x0A:
//	case 0x05:
//		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v0], g.p[v3], g.val[v0], g.val[v3]);
//		ntri++;
//		tri[1].p[0] = tri[0].p[0];
//		tri[1].p[1] = VertexInterp(iso, g.p[v1], g.p[v2], g.val[v1], g.val[v2]);
//		tri[1].p[2] = tri[0].p[1];
//		ntri++;
//		break;
//	case 0x09:
//	case 0x06:
//		tri[0].p[0] = VertexInterp(iso, g.p[v0], g.p[v1], g.val[v0], g.val[v1]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v1], g.p[v3], g.val[v1], g.val[v3]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v2], g.p[v3], g.val[v2], g.val[v3]);
//		ntri++;
//		tri[1].p[0] = tri[0].p[0];
//		tri[1].p[1] = VertexInterp(iso, g.p[v0], g.p[v2], g.val[v0], g.val[v2]);
//		tri[1].p[2] = tri[0].p[2];
//		ntri++;
//		break;
//	case 0x07:
//	case 0x08:
//		tri[0].p[0] = VertexInterp(iso, g.p[v3], g.p[v0], g.val[v3], g.val[v0]);
//		tri[0].p[1] = VertexInterp(iso, g.p[v3], g.p[v2], g.val[v3], g.val[v2]);
//		tri[0].p[2] = VertexInterp(iso, g.p[v3], g.p[v1], g.val[v3], g.val[v1]);
//		ntri++;
//		break;
//	}
//
//	return(ntri);
//}

typedef struct {
   XYZ vp;              /* View position           */
   XYZ vd;              /* View direction vector   */
   XYZ vu;              /* View up direction       */
   XYZ pr;              /* Point to rotate about   */
   double focallength;  /* Focal Length along vd   */
   double aperture;     /* Camera aperture         */
   double eyesep;       /* Eye separation          */
	int screenheight,screenwidth;
} CAMERA;

extern CAMERA camera;

/* Number of bins in the historgram */
#define HISTLENGTH 256

typedef struct {
	short int *data;
	int Nx,Ny,Nz;
	double Dx,Dy,Dz;
	int Rx,Ry,Rz;
	int themin,themax;
	int resolution;
	int histogram[HISTLENGTH];
	int isolevel;
} VOLUME;

#define KEYBOARDCONTROL 0
#define MOUSECONTROL    1

/* See colour ramp menu */
#define CONSTANTCOLOUR 100

/* Maximum drawing elements */
#define MAXPOINTS  500000
#define MAXLINES   500000
#define MAXPOLYGON 500000

/* Degrees of dirty */
#define NOTDIRTY     0
#define ENVDIRTY     1
#define GEOMDIRTY    2

/* Rendering modes  - these numbers match the menu numbers! */
#define POINTSABOVE  1
#define POINTSBELOW  2
#define LINESABOVE   3
#define LINESBELOW   4
#define ISOPOINTS    5
#define ISOSURFACE   6
#define RENDER       7

void HandleDisplay(void);
void CreateEnvironment(void);
void CreateWorld(void);
void MakeGeometry(void);
void MakeLighting(void);
void DrawExtras(void);
void MakeEnv(void);
int  ReadVolume(char *);
int  SaveVolume(char *);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleSpecialKeyboard(int key,int x, int y);
void HandleMouse(int,int,int,int);
void HandleVisibility(int vis);
void HandleReshape(int,int);
void HandleMouseMotion(int,int);
void HandlePassiveMotion(int,int);
void HandleTimer(int);
void RotateCamera(double,double,double,int);
void TranslateCamera(int,int);
void CameraHome(int);
void FlyCamera(int);
void MallocGrid(int,int,int);
void CalcBounds(void);
void UpdateResolution(int);

/* Drawing */
void DrawTheAxes(void);
void DrawGrid(void);
void DrawHistogram(int);
void HistogramClick(int,int);
void DrawPoints(int,int,int);
void DrawLines(int,int,int);
void DrawIsopoints(int,int);
void DrawInfo(int,int);
void DrawIsoSurface(int); 
XYZ  CalcGridNormal(int,int,int);
void SendGridVertex(int,int,int);

/* Menus */
void CreateMenus(void);
void HandleCameraMenu(int);
void HandleDataMenu(int);
void HandleColourMenu(int);
void HandleRenderMenu(int);
void HandlePointMenu(int);
void HandleLightMenu(int);
void HandleToggleMenu(int);
void HandleDrawMenu(int);
void HandleResolMenu(int);
void HandleMainMenu(int);

/* Usage */
void GiveUsage(char *);
void DrawUsage(int,int);
