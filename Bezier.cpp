#include <stdio.h>

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Bezier.h"

template <typename T>
Bezier<T>::Bezier(int resolution) : resolution(resolution)
{
  origin = 0;
  drawCage = drawCurve = true;
  unsetSelectedPoint(); //indicates no control point selected
}

template <typename T>
Point<T> Bezier<T>::evaluateCurve(Point<T> p0,
                               Point<T> p1,
                               Point<T> p2,
                               Point<T> p3,
                               float t)
{
  //compute our 3-dimensional coefficients
  Point<T> a = -p0 + 3 * p1 - 3 * p2 + p3;
  Point<T> b = 3 * p0 - 6 * p1 + 3 * p2;
  Point<T> c = -3 * p0 + 3 * p1;

  float t2 = t * t; //reduce the recomputations of powers of t in the next line

  return a * (t2*t) + b * t2 + c * t + p0;
}


//Returns the point that is a percentage of the total curve length
template <typename T>
Point<T> Bezier<T>::getPercentageAlongCurve(float percentage)
{
  std::map<double, float>::iterator it = distanceTable.end();
  it--; //back up, since end() is actually past the end
  double totalDist = (*it).first;
  double targetDist = totalDist * percentage;

  std::pair<double, float> lower, upper;
  //find the closet pair of points in our distance table to our target and lerp
  it = distanceTable.upper_bound(targetDist);
  if(it != distanceTable.begin())
    it--;
  lower = *it;
  upper = *(++it);

  double innerdist = upper.first - lower.first;
  double innertime = upper.second - lower.second;

  //What % of distance between our two samples is our target distance?
  double proportion = (targetDist - lower.first) / innerdist;

  //Our target time value will be the same proportion
  float t = proportion * innertime + lower.second;

  int ctrlPtGroup = t * 1; //intentionally truncate to figure out which set of 4 control points to use

  return evaluateCurve(controlPoints.at(ctrlPtGroup * 3),
                       controlPoints.at(ctrlPtGroup * 3 + 1),
                       controlPoints.at(ctrlPtGroup * 3 + 2),
                       controlPoints.at(ctrlPtGroup * 3 + 3),
                       t - ctrlPtGroup * 1.0f);
}

template <typename T>
void Bezier<T>::setSelectedPoint(unsigned int index)
{
  if(index < controlPoints.size())
    selectedPoint = index;
  else
    //invalid index default to no selection
    unsetSelectedPoint();
}


template <typename T>
void Bezier<T>::unsetSelectedPoint()
{
  selectedPoint = -1;
}


template <typename T>
void Bezier<T>::moveSelectedPoint(Vector<T> v)
{
  //Applies a vector to the control point to move it
  if(selectedPoint >= 0)
  {
    Point<T>& p = controlPoints.at(selectedPoint);
    p += v;

    //Since a control point changed, we must repopulate the
    //distance table
    populateDistanceTable();
  }
}


template <typename T>
void Bezier<T>::toggleControlCageVisible()
{
  drawCage = !drawCage;
}


template <typename T>
void Bezier<T>::toggleCurveVisible()
{
  drawCurve = !drawCurve;
}


template <typename T>
bool Bezier<T>::loadControlPoints(const char* const filename)
{
  FILE* csv = fopen(filename, "r");

	if(!csv)
  {
    fprintf(stderr, "Error opening %s\n", filename);
    return false;
  }

  int npoints;
  fscanf(csv, "%d ", &npoints);
  if(npoints < 4)
  {
    fprintf(stderr, "%d is too few points to specify a Bezier curve\n", npoints);
    return false;
  }


  float x, y, z;

  for(int i = 0; i < npoints; ++i)
  {
    //read a point
    fscanf(csv, "%f,%f,%f ", &x, &y, &z);
    printf("Read point %f %f %f\n", x, y, z);
    controlPoints.push_back(Point<GLfloat>(x, y, z));
  }

  fclose(csv);

  populateDistanceTable();
  
  return true;
}

template <typename T>
void Bezier<T>::saveControlPoints()
{
  FILE* fd = fopen("customPoints.csv", "w");
  if(!fd)
  {
    fprintf(stderr, "Could not open file for writing!\n");
    return;
  }

  int npoints = controlPoints.size();
  fprintf(fd, "%d\n", npoints);

  Point<T> p;
  for(int i = 0; i < npoints; ++i)
  {
    p = controlPoints.at(i);
    fprintf(fd, "%f, %f, %f\n", p.getX(), p.getY(), p.getZ());
  }
  fclose(fd);
}


template <typename T>
bool Bezier<T>::valid()
{
  return controlPoints.size() > 3;
}

template <typename T>
void Bezier<T>::render(bool selectionMode)
{
  glPushMatrix();
  {
    int npoints = controlPoints.size();
    if(origin)
      glTranslatePoint(*origin); //place in worldspace

    drawControlPoints(selectionMode, npoints);

    if(selectionMode) return; //break out early

    if(drawCage)
      drawControlCage(npoints);

    if(drawCurve)
    {
      int i = 0;
      while(i + 3 < npoints)
      {
        renderCurve(controlPoints.at(i),
                          controlPoints.at(i + 1),
                          controlPoints.at(i + 2),
                          controlPoints.at(i + 3));
        i += 3; //share the endpoint with the last curve to be c0
      }
    }
  }
  glPopMatrix();
}


template <typename T>
void Bezier<T>::renderCurve(Point<T> p0,
                         Point<T> p1,
                         Point<T> p2,
                         Point<T> p3)
{
  glDisable( GL_LIGHTING );
  glPushMatrix();
    glColor3f(0, 0, 1); //Render curves in blue
    glLineWidth(3.0f);


    glBegin(GL_LINE_STRIP);
    for(double t = 0.0; t < 1.0; t += 1.0 / resolution)
      glVertexPoint(evaluateCurve(p0, p1, p2, p3, t));

    glEnd();
  glPopMatrix();

  glLineWidth(1.0f);
  glEnable( GL_LIGHTING );
}


template <typename T>
void Bezier<T>::drawControlCage(int npoints)
{
  glDisable( GL_LIGHTING );
  glColor3ub(226, 223, 49); //Yellow for cage lines
  glLineWidth(3.0f);	// make our grid lines a little thicker so they are easier to see

  Point<T> p;
  glBegin( GL_LINE_STRIP );
  for(int i = 0; i < npoints; ++i)
  {
    p = controlPoints.at(i);
    glVertexPoint(p);
  }
  glEnd();
  glLineWidth(1.0f);

  glEnable( GL_LIGHTING );
}


template <typename T>
void Bezier<T>::drawControlPoints(bool selectionMode, int npoints)
{
  Point<T> p;
  glColor3ub(39, 122, 53); //green spheres
  for(int i = 0; i < npoints; ++i)
  {
    p = controlPoints.at(i);
    glPushMatrix();
      glTranslatePoint(p);

      //assign this object a pickable name in pick mode
      if(selectionMode)
        glLoadName(i);
      else
      {
        //don't bother changing colors in our selection pass
        if(i == selectedPoint)
          glColor3ub(122, 68, 111); //purple
        else
          glColor3ub(39, 122, 53); //green spheres
      }

      glutSolidSphere(0.2, 10, 4);
    glPopMatrix();
  }
}

template <typename T>
void Bezier<T>::populateDistanceTable()
{
  distanceTable.clear();
  //At t = 0, our distance will always be zero
  distanceTable.insert(std::pair<double, float>(0.0, 0.f));

  //A set of control points that defines a since cubic Bezier
  Point<T> p0 = controlPoints.at(0);
  Point<T> p1 = controlPoints.at(1);
  Point<T> p2 = controlPoints.at(2);
  Point<T> p3 = controlPoints.at(3);

  //Two points on our curve
  Point<T> a;
  Point<T> b(p0); //our starting point of the curve


  int npoints = controlPoints.size();
  int rootPoint = 0;
  int section = 0; //defines which set of 4 control points we're examining
  double distanceAccumulator = 0.0;

  float t = 1.0 / resolution;
  while(rootPoint + 3 < npoints)
  {
    section = rootPoint / 3;
    p0 = controlPoints.at(rootPoint);
    p1 = controlPoints.at(rootPoint + 1);
    p2 = controlPoints.at(rootPoint + 2);
    p3 = controlPoints.at(rootPoint + 3);

    for(; t < section + 1.0; t += 1.0 / resolution)
    {
      a = b; //shift the last computed point over
      b = evaluateCurve(p0, p1, p2, p3, t - section * 1.0); //be suer to keep 0 < t < 1 for each section
      distanceAccumulator += distanceBetween(a, b);
      distanceTable.insert(std::pair<double, float>(distanceAccumulator, t));
    }
      
    rootPoint += 3; //move onto the next set of control points
  }
}


template class Bezier<float>; //Generates all template functions for use with float/GLfloat
