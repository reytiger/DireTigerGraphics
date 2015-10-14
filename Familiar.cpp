#include "Familiar.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//ctor
Familiar::Familiar() : path(100), percentage(0.f), speed(0.005), center(0) {};

void Familiar::draw(bool selectionMode)
{
  //only draw something if the bezier has control points
  if(path.valid())
  {
    if(center)
    {
      glTranslatePoint(*center); //position around our object
      glRotatef(*rot, 0, 1, 0);
    }

    path.render();

    glColor3f(1.f, 0.f, 0.f);
    glPushMatrix();

      glTranslatePoint(path.getPercentageAlongCurve(percentage));
      glScalef(0.3f, 0.3f, 0.3f);
      glutSolidIcosahedron();
    glPopMatrix();
  }
}


void Familiar::tick()
{
  //update the position along the path
  percentage += speed;
  if(percentage > 1.0f)
    percentage -= 1.0f;
}


void Familiar::attachToObj(Point<GLfloat>* obj, float* rotation)
{
  center = obj;
  rot = rotation;
}
