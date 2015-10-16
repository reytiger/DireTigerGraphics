#include "Light.h"
#include <cstdio>

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#define GL_LIGHTN GL_LIGHT0 + glLightNum
//ctors
Light::Light() : type(OMNI),
                 position(Point<float>(0.f, 0.f, 0.f)),
                 direction(Vector<float>(0.f, 0.f, -1.f)),
                 diffuseCol(Color(1.f, 1.f, 1.f, 1.f)),
                 specularCol(Color(1.f, 1.f, 1.f, 1.f)),
                 ambientCol(Color(1.f, 1.f, 1.f, 1.f)),
                 angleCutoff(45.f),
                 distanceFalloff(100.f)
{
  //be sure ligthing is enabled
  glEnable(GL_LIGHTING);
  assignGLLightNum();
  setGLLightProperties();
  glEnable(GL_LIGHTN); //enable the new light
};

Light::Light(LightType type,
             const Point<float>& pos,
             const Color& diff,
             const Color& spec,
             const Color& ambi,
             const Vector<float> dir) :
             type(type),
             position(pos),
             direction(dir),
             diffuseCol(diff),
             specularCol(spec),
             ambientCol(ambi)
{
  assignGLLightNum();
  setGLLightProperties();
  glEnable(GL_LIGHTN); //enable the new light
};


Light::~Light()
{
  //mark this light as unused
  lightNums[glLightNum] = false;
  glDisable(GL_LIGHTN);
}



void Light::setColorDiff(const Color& col)
{
  diffuseCol = col;
  glLightfv(GL_LIGHTN, GL_DIFFUSE, col.rgba);
};


void Light::setColorAbmi(const Color& col)
{
  ambientCol = col;
  glLightfv(GL_LIGHTN, GL_AMBIENT, col.rgba);
};


void Light::setColorSpec(const Color& col)
{
  specularCol = col;
  glLightfv(GL_LIGHTN, GL_SPECULAR, col.rgba);
};


void Light::setPosition(const Point<float>& pos)
{
  position = pos;
}


void Light::setDirection(const Vector<float>& other)
{
  direction = other;
}


void Light::setSpotParams(const Vector<float>& direction, float angle, float falloff)
{
  setDirection(direction);
  angleCutoff = angle;
  distanceFalloff = falloff;
  if(type == SPOT)
    applySpotParams();
}


void Light::changeType(LightType newType)
{
  type = newType;
  if(type == SPOT)
    applySpotParams();
}


void Light::assignGLLightNum()
{
  //find a free entry in the lights list
  for(int i = 0; i < NUM_LIGHTS; ++i)
  {
    if(!lightNums[i])
    {
      lightNums[i] = true;
      glLightNum = i;
      return;
    }
  }
  fprintf(stderr, "Warning: too many GL lights!");
}


void Light::setGLLightProperties()
{
  switch(type)
  {
  case SPOT:
    applySpotParams();
  case OMNI:
    updatePosition();
  case DIRECTED:
    updateDirection();
  }

  updateColor();
}


void Light::updateColor()
{
  //applys the current coloration settings
  glLightfv(GL_LIGHTN, GL_DIFFUSE, diffuseCol.rgba);
  glLightfv(GL_LIGHTN, GL_SPECULAR, specularCol.rgba);
  glLightfv(GL_LIGHTN, GL_AMBIENT, ambientCol.rgba);
}

void Light::updatePosition()
{
  float posv[4];
  posv[0] = position.getX();
  posv[1] = position.getY();
  posv[2] = position.getZ();
  posv[3] = 1.f;

  glLightfv(GL_LIGHTN, GL_POSITION, posv);
}


void Light::updateDirection()
{
  float dir[4];
  dir[0] = direction.getX();
  dir[1] = direction.getY();
  dir[2] = direction.getZ();
  switch(type)
  {
  case OMNI:
    dir[3] = 0.f;
    break;
  case DIRECTED:
    dir[3] = 0.f;
    glLightfv(GL_LIGHTN, GL_POSITION, dir);
    break;
  case SPOT:
    dir[3] = 1.f;
    glLightfv(GL_LIGHTN, GL_SPOT_DIRECTION, dir);
    break;
  }
}
void Light::applySpotParams()
{
  updateDirection();
  glLightf(GL_LIGHTN, GL_SPOT_CUTOFF, angleCutoff);
  glLightf(GL_LIGHTN, GL_SPOT_EXPONENT, distanceFalloff);
}

//The static array indicating which light numbers are free
const int Light::numLights = NUM_LIGHTS;
bool Light::lightNums[NUM_LIGHTS] = {false, false, false, false, false, false, false, false};
