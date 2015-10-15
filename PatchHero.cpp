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

PatchHero::PatchHero(BezPatch<float>& patch, int subPatch, const float u, const float v) :
                     myPatch(patch), subPatch(subPatch), u(u), v(v) {};


void PatchHero::setU(const float newU)
{
  float tmp = newU;
  if(newU < 0.f)
  {
    do
    {
      //don't go any further if this is the 0th patch
      if(prevSubPatch())
        tmp++;
      else
        tmp = 0.f;
    }
    while(tmp < 0.f);
  }
  else if(newU > 1.f)
  {
    do
    {
      //
      if(nextSubPatch())
        tmp--;
      else
        tmp = 1.f;
    }
    while(tmp > 1.f);
  }
  u = tmp;
}


void PatchHero::setV(const float newV)
{
  float tmp = newV;
  if(newV < 0.f)
  {
    do
    {
      if(prevSubPatch())
        tmp++;
      else
        tmp = 0.f;
    }
    while(tmp < 0.f);
  }
  else if(newV > 1.f)
  {
    do
    {
      if(nextSubPatch())
        tmp--;
      else
        tmp = 1.f;
    }
    while(tmp > 1.f);
  }
  v = tmp;
}


void PatchHero::incU(const float amt)
{
  setU(u + amt);
}


void PatchHero::incV(const float amt)
{
  setV(v + amt);
}


//indicates if the next subpatch was reached
bool PatchHero::nextSubPatch()
{
  //subpatch is an index, so offset
  if(myPatch.getNSubPatches() - 1 > subPatch)
  {
    ++subPatch;
    return true;
  }
  return false;
}


bool PatchHero::prevSubPatch()
{
  if(subPatch > 0)
  {
    --subPatch;
    return true;
  }
  return false;
}


void PatchHero::render(bool selectionMode)
{
  if(selectionMode) return;

  //update the position on the patch
  myPatch.placeOnSurface(*this, subPatch, u, v);

  //set the render context to the proper location
  glEmplaceObject();

  Basis<float> basis = myPatch.getBasis(subPatch, u, v);

  glPushMatrix();
    glLineWidth(3.0f);
    //draw the basis
    glTranslatef(0.f, 2.f, 0.f);
    glColor3ub(226, 229, 39);
    (basis.x).draw();
    glColor3ub(39, 216, 229);
    (basis.y).draw();
    glColor3ub(140, 37, 131);
    basis.z.draw();
    glLineWidth(1.0f);
  glPopMatrix();

  glOrientObject();

  glRotateToBasis(basis, Vector<float>(0.f, 2.f, 0.f));

  //offset off the surface
  //glTranslatefVector(normal * 0.5f);

  //draw the hero
  glScalef(2.f, 1.f, 1.f);
  glutSolidCube(1);
}


void PatchHero::tick()
{
  //increase the yaw
  //orientation.setYaw(orientation.getYaw() + 1.f);
  //orientation.setPitch(orientation.getPitch() + 1.f);
  //orientation.setRoll(orientation.getRoll() + 1.f);
}
