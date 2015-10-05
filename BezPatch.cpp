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

template <typename T>
BezPatch<T>::BezPatch(int resolution) : resolution(resolution) {};


template <typename T>
bool BezPatch<T>::loadControlPoints(const char* const filename)
{
  FILE* csv = fopen(filename, "r");

	if(!csv)
  {
    fprintf(stderr, "Error opening %s\n", filename);
    return false;
  }

  int npoints;
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
      if(fscanf(csv, "%f,%f,%f ", &x, &y, &z) != 3)
        fprintf(stderr, "Warning: didn't read complete coordinate for BezPatch\n");

      printf("Read point %f %f %f\n", x, y, z);
      ctrlPoints.push_back(Point<GLfloat>(x, y, z));
    }
  }


  fclose(csv);
  
  return true;
}


template <typename T>
void BezPatch<T>::render()
{
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
    }

    //Our first axis
    for(float u = 0.f; u <= 1.f; u += 1.f / resolution)
    {
      //calculate the control points for the v bezier
      Point<T> arr[1] = {uBez[0].evalutateCurve(u),
                         uBez[1].evalutateCurve(u),
                         uBez[2].evalutateCurve(u),
                         uBez[3].evalutateCurve(u)};
      Bezier<T> vBez(resolution, arr);
      for(float v = 0.f; v <= 1.f; v += 1.f / resolution)
      {
        //Draw a point on the surface
        glVertexPoint(vBez.evaluateCurve(v));
      }
    }
  }
}
