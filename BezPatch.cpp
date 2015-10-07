#include <cstdio>
#include "BezPatch.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//Ctors
template <typename T>
BezPatch<T>::BezPatch(int resolution) : resolution(resolution), origin(0, 0, 0), pointsLoaded(false) {};


template <typename T>
BezPatch<T>::BezPatch(int resolution, const Point<T>& startPos) : resolution(resolution), origin(startPos), pointsLoaded(false) {};


template <typename T>
bool BezPatch<T>::loadControlPoints(const char* const filename, float scaling)
{
  FILE* csv = fopen(filename, "r");

	if(!csv)
  {
    fprintf(stderr, "Error opening %s\n", filename);
    return false;
  }

  if(fscanf(csv, "%d ", &numPatches) != 1)
  {
    fprintf(stderr, "Unable to read number of BezPatches in file!\n");
    fclose(csv);
    return false;
  }

  for(int patch = 0; patch < numPatches; ++patch)
  {
    float x, y, z;

    //Read the first patch
    for(int i = 0; i < 16; ++i)
    {
      //read a point
      int stat = fscanf(csv, "%f, %f, %f ", &x, &y, &z);
      if(stat != 3)
        fprintf(stderr, "Warning: didn't read complete coordinate for BezPatch\n");

      printf("Read point %f %f %f\n", x, y, z);
      ctrlPoints.push_back(Point<GLfloat>(x * scaling, y * scaling, z * scaling));
    }
  }


  fclose(csv);
  pointsLoaded = true;
  
  return true;
}


template <typename T>
void BezPatch<T>::render()
{
  //Don't bother rendering if we have no point data
  if(!pointsLoaded)
    return;

  glTranslatePoint(origin);
  for(int p = 0; p < numPatches; ++p)
  {
    //construct bezier curves that run along the u axis
    Bezier<T> uBez[4];
    for(int i = 0; i < 4; ++i)
    {
      Point<T> a = ctrlPoints.at(p * 16 + i * 4);
      Point<T> b = ctrlPoints.at(p * 16 + i * 4 + 1);
      Point<T> c = ctrlPoints.at(p * 16 + i * 4 + 2);
      Point<T> d = ctrlPoints.at(p * 16 + i * 4 + 3);
      Point<T> arr[4] = {a, b, c, d};
      uBez[i] = Bezier<T>(resolution, arr);

      //draw the u curves so we can see control cages
      uBez[i].render(false);
    }
    glColor3f(1.f, 1.f, 1.f);

    float u = 0.f;
    float v = 0.f;
    const float step = 1.f / resolution;

    //iterate through the u axis at the given resolution
    for(int i = 0; i < resolution; ++i)
    {
      //inverse of resolution will tell us how what % of the curve
      //length is each quad's dimensions
      u = i * step; 

      //calculate the control points for two v beziers
      //which will determine points along each edge of the quad
      Point<T> arr[] = {uBez[0].getPercentageAlongCurve(u),
                        uBez[1].getPercentageAlongCurve(u),
                        uBez[2].getPercentageAlongCurve(u),
                        uBez[3].getPercentageAlongCurve(u)};
      Bezier<T> vBezNear(resolution, arr);
      Point<T> arr2[] = {uBez[0].getPercentageAlongCurve(u + step),
                         uBez[1].getPercentageAlongCurve(u + step),
                         uBez[2].getPercentageAlongCurve(u + step),
                         uBez[3].getPercentageAlongCurve(u + step)};
      Bezier<T> vBezFar(resolution, arr2);

      //printf("u: %3.2f, u + stp: %3.2f\n", u, u + step);
      glPushMatrix();
        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= resolution; ++j)
        {
          v = j * step;
          //printf("\tv: %3.2f\n", v);
          //Draw a quad on the surface
          glVertexPoint(vBezFar.getPercentageAlongCurve(v));
          glVertexPoint(vBezNear.getPercentageAlongCurve(v));
        }
        glEnd();
      glPopMatrix();
    }
  }
}


template <typename T>
void BezPatch<T>::setOrigin(const Point<T>& p)
{
  origin = p;
}

template class BezPatch<float>; //special instance this template
