#include "opengllib.h"

void DrawGLText(float loc_x, float loc_y, const char *string) {
	glPushAttrib(GL_CURRENT_BIT);
	glColor3ub(255, 255, 255);
	while (*string != '\0') {
		glRasterPos2i(loc_x, loc_y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string);
		loc_x += (glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, *string) + 1);
		string++;
	}
}

void CreateBox(XYZ min, XYZ max, int c)
{
	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);

	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();
}
