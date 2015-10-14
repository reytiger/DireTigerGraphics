#include "PatchHero.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

PatchHero::PatchHero(BezPatch<float>& patch, const float u, const float v) : myPatch(patch), u(u), v(v) {};


void PatchHero::setU(const float newU)
{
  if(newU < 0.f)
  {
    u = 0.f;
    return;
  }
  else if(newU > 1.f)
  {
    u = 1.f;
    return;
  }
  else
    u = newU;
}


void PatchHero::setV(const float newV)
{
  if(newV < 0.f)
  {
    v = 0.f;
    return;
  }
  else if(newV > 1.f)
  {
    v = 1.f;
    return;
  }
  v = newV;
}


void PatchHero::incU(const float amt)
{
  setU(u + amt);
}


void PatchHero::incV(const float amt)
{
  setV(v + amt);
}

void PatchHero::render(bool selectionMode)
{
  if(selectionMode) return;

  //update the position on the patch
  myPatch.placeOnSurface(*this, 0, u, v);

  //set the render context to the proper location
  glEmplaceObject();

  Basis<float> basis = myPatch.getBasis(0, u, v);
  glRotateToBasis(basis);

  //glOrientObject();

  //offset off the surface
  //glTranslatefVector(normal * 0.5f);

  //draw the hero
  glutSolidCube(1);
}
