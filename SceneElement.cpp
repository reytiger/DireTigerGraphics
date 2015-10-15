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
                               orientation(0.f, 0.f, 0.f),
                               material() {};

SceneElement::SceneElement(const Point<float>& pos, const Orientation<float>& ori, const Material& mat) :
                           position(pos), orientation(ori), material(mat) {};

Point<float> SceneElement::getPosition() const
{
  return position;
}

Orientation<float> SceneElement::getOrientation() const
{
  return orientation;
}


void SceneElement::setPosition(const Point<float>& pos)
{
  position = pos;
}

void SceneElement::setOrientation(const Orientation<float>& ori)
{
  orientation = ori;
}

void SceneElement::setMaterial(const Material& mat)
{
  material = mat;
}


void SceneElement::glSetupRender()
{
  glEmplaceObject();
  glOrientObject();
  glApplyMaterial();
}


//rotates the openGL context according to the position and orientation data
void SceneElement::glEmplaceObject()
{
  
  glMatrixMode(GL_MODELVIEW);
  //This should probably accept the scene up vector from our main, but
  //having it hardcoded doesn't seem unreasonable
  glTranslatePoint(position);
}


void SceneElement::glOrientObject()
{
  glMatrixMode(GL_MODELVIEW);
  orientation.glOrient();
  //TODO:
  //rotate the object so it has the correct tilt
  //glRotatefVector(-normal.angleTo(up), normal.cross(up));
  //and rotate around the normal by our heading
  //glRotatefVector(theta, normal);
}


void SceneElement::glApplyMaterial()
{
  const GLenum face = GL_FRONT_AND_BACK;
  glMaterialfv(face, GL_DIFFUSE, material.getDiffuseColor());
  glMaterialfv(face, GL_SPECULAR, material.getSpecularColor());
  glMaterialfv(face, GL_AMBIENT, material.getAmbientColor());
}


void SceneElement::render()
{
  //TODO
  //render orientation of this object
}

void SceneElement::tick() {}

const Vector<float> SceneElement::up = Vector<float>(0.f, 1.f, 0.f);
