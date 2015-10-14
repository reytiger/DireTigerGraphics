#include "SceneElement.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//ctors
SceneElement::SceneElement() : position(Point<float>(0.f, 0.f, 0.f)),
                               normal(Vector<float>(0.f, 1.f, 0.f)),
                               theta(0.f) {};

SceneElement::SceneElement(const Point<float>& pos, const Vector<float>& norm, const float heading) :
                           position(pos), normal(norm), theta(heading) {};

Point<float> SceneElement::getPosition() const
{
  return position;
}

Vector<float> SceneElement::getNormal() const
{
  return normal;
}


float SceneElement::getHeading() const
{
  return theta;
}

void SceneElement::setPosition(const Point<float>& pos)
{
  position = pos;
}

void SceneElement::setNormal(const Vector<float>& norm)
{
  normal = norm;
}


void SceneElement::setHeading(const float degrees)
{
  theta = degrees;
}


//rotates the openGL context according to the position and orientation data
void SceneElement::glEmplaceObject()
{
  
  glMatrixMode(GL_MODELVIEW);
  //This should probably accept the scene up vector from our main, but
  //having it hardcoded doesn't seem unreasonable
  glRotatefVector(theta, normal);
}
