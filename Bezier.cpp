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


//ctors
template <typename T>
SubCurve<T>::SubCurve() : resolution(10)
{
  origin = 0;
}

template <typename T>
SubCurve<T>::SubCurve(const int resolution, const Point<T> (&points)[4], const double startDist) : resolution(resolution)
{
  origin = 0;
  for(int i = 0; i < 4; ++i)
    controlPoints.push_back(points[i]);
  populateDistanceTable(startDist);
}


template <typename T>
Point<T> SubCurve<T>::evaluateCurve(Point<T> p0,
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

  return a * (t2 * t) + b * t2 + c * t + p0;
}

template <typename T>
Point<T> SubCurve<T>::evaluateCurve(float t)
{
  return evaluateCurve(controlPoints.at(0), controlPoints.at(1),
                       controlPoints.at(2), controlPoints.at(3), t);
}


template <typename T>
Point<T> SubCurve<T>::getPercentageAlongCurve(float percentage)
{
  return evaluateCurve(calcTforPercentDistance(percentage));
}


template <typename T>
Vector<T> SubCurve<T>::getTangent(Point<T> p0,
                                Point<T> p1,
                                Point<T> p2,
                                Point<T> p3,
                                float t)
{
  //compute our 3-dimensional coefficients
  Point<T> a = -p0 + 3 * p1 - 3 * p2 + p3;
  Point<T> b = 3 * p0 - 6 * p1 + 3 * p2;
  Point<T> c = -3 * p0 + 3 * p1;
  
  Point<T> res = 3 * a * t * t + 2 * b * t + c;
  return Vector<T>(res.getX(), res.getY(), res.getZ());
}


template <typename T>
Vector<T> SubCurve<T>::getTangent(float t)
{
	return getTangent(controlPoints.at(0), controlPoints.at(1),
                    controlPoints.at(2), controlPoints.at(3), t);
}


template <typename T>
Vector<T> SubCurve<T>::getTangentByPercentage(float percentage)
{
  return getTangent(calcTforPercentDistance(percentage));
}



template <typename T>
double SubCurve<T>::getEndDistance()
{
  std::map<double, float>::iterator i = distanceTable.end();
  i--;
  return (*i).first;
}


template <typename T>
void SubCurve<T>::render(bool drawCtrlCage, bool drawCurve)
{
  glPushMatrix();
  {
    if(origin)
      glTranslatePoint(*origin); //place in worldspace

    if(drawCtrlCage)
    {
      drawControlPoints();
      drawControlCage();
    }

    if(drawCurve)
    {
      //draw the actual curve
      glDisable( GL_LIGHTING );
      glPushMatrix();
        glColor3f(0, 0, 1); //Render curves in blue
        glLineWidth(3.0f);

        glBegin(GL_LINE_STRIP);
        const float step = 1.f / resolution;
        float t;
        for(int i = 0; i <= resolution; ++i)
        {
          t = step * i;
          glVertexPoint(evaluateCurve(t));
        }

        glEnd();
      glPopMatrix();

      glLineWidth(1.0f);
      glEnable( GL_LIGHTING );
    }
  }
  glPopMatrix();
}


template <typename T>
void SubCurve<T>::getCtrlPoints(Point<T> (&arr)[4])
{
  //write the point data into the provided array
  if(sizeof(arr) / sizeof(T) == 4)
    for(int i = 0; i < 4; ++i)
      arr[i] = controlPoints.at(i);
  else
    fprintf(stderr, "Size of provided control point array is wrong size!\n");
}


template <typename T>
void SubCurve<T>::setOrigin(Point<T>* pt)
{
  origin = pt;
}


//return a point on the subcurve for some percentage of its total length
template <typename T>
float SubCurve<T>::calcTforPercentDistance(float percentage)
{
  //don't allow negative percentages
  if(percentage < 0.f)
    return 0.f;
  //lock at 100%
  if(percentage >= 1.f)
    return 1.f;

  double targetDist = getEndDistance() * percentage;

  std::pair<double, float> lower, upper;

  //find the closet pair of points in our distance table to our target and lerp
  std::map<double, float>::iterator it = distanceTable.upper_bound(targetDist);
  if(it != distanceTable.begin())
    it--;
  lower = *it;
  upper = *(++it);

  double innerdist = upper.first - lower.first;
  double innertime = upper.second - lower.second;

  //What % of distance between our two samples is our target distance?
  double proportion = (targetDist - lower.first) / innerdist;

  //Our target time value will be the same proportion
  return proportion * innertime + lower.second;
}


//Samples the curve to generate a table mapping distance along the curve to a t input
//StartDist is an offset determining how far along the overall curve set this subcurve starts at
template <typename T>
void SubCurve<T>::populateDistanceTable(double startDist)
{
  distanceTable.clear();
  //At t = 0, our distance will always be at our starting distance
  distanceTable.insert(std::pair<double, float>(startDist, 0.f));

  //A set of control points that defines a since cubic Bezier
  Point<T> p0 = controlPoints.at(0);
  Point<T> p1 = controlPoints.at(1);
  Point<T> p2 = controlPoints.at(2);
  Point<T> p3 = controlPoints.at(3);

  //Two points on our curve
  Point<T> a;
  Point<T> b(p0); //our starting point of the curve

  double distanceAccumulator = startDist;
  
  //use half the rendering resolution as our step size
  float sampleDensity = resolution / 2.f;

  float t = 0.f;
  const float step = 1.f / sampleDensity;

  for(int i = 0; i <= sampleDensity; ++i)
  {
    t = i * step;
    a = b; //shift the last computed point over
    b = evaluateCurve(p0, p1, p2, p3, t); //Find the next point
    distanceAccumulator += distanceBetween(a, b);
    distanceTable.insert(std::pair<double, float>(distanceAccumulator, t));
  }
}


template <typename T>
void SubCurve<T>::drawControlCage()
{
  glDisable( GL_LIGHTING );
  glColor3ub(226, 223, 49); //Yellow for cage lines
  glLineWidth(3.0f);	// make our grid lines a little thicker so they are easier to see

  Point<T> p;
  glBegin( GL_LINE_STRIP );
  for(int i = 0; i < 4; ++i)
  {
    p = controlPoints.at(i);
    glVertexPoint(p);
  }
  glEnd();
  glLineWidth(1.0f);

  glEnable( GL_LIGHTING );
}


template <typename T>
void SubCurve<T>::drawControlPoints()
{
  Point<T> p;
  glColor3ub(39, 122, 53); //green spheres
  for(int i = 0; i < 4; ++i)
  {
    p = controlPoints.at(i);
    glPushMatrix();
      glTranslatePoint(p);
      glutSolidSphere(0.2, 10, 4);
    glPopMatrix();
  }
}

//-----Bezier Group class below------

template <typename T>
Bezier<T>::Bezier() : resolution(10)
{
  drawCage = drawCurve = true;
}


template <typename T>
Bezier<T>::Bezier(const int resolution) : resolution(resolution)
{
  drawCage = drawCurve = true;
}


template <typename T>
Bezier<T>::Bezier(const int resolution, const Point<T> (&points)[4]) : resolution(resolution)
{
  drawCage = drawCurve = true;
  subCurves.push_back(SubCurve<T>(resolution, points, 0.0)); //construct a single subcurve
}


template <typename T>
void Bezier<T>::render()
{
  SubCurve<T>* curve;
  //render each subcurve
  for(typename std::vector<SubCurve<T> >::iterator i = subCurves.begin(); i != subCurves.end(); ++i)
  {
    curve = &*i;
    curve->render(drawCage, drawCurve);
  }
}


//Returns the point that is a percentage of the total curve length
template <typename T>
Point<T> Bezier<T>::getPercentageAlongCurve(float percentage)
{
  std::pair<int, float> t = calcTforPercentDistance(percentage);

  return subCurves.at(t.first).evaluateCurve(t.second);
}


template <typename T>
Vector<T> Bezier<T>::getTangentByPercentage(float percentage)
{
  std::pair<int, float> t = calcTforPercentDistance(percentage);

  return subCurves.at(t.first).getTangent(t.second);
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

  int nPoints;
  int nSubCurves;
  fscanf(csv, "%d ", &nPoints);

  if(nPoints < 4)
  {
    fprintf(stderr, "%d is too few points to specify a Bezier curve\n", nPoints);
    return false;
  }

  //calculate the number of subcurves
  nSubCurves = 1 + (nPoints - 4) / 3;
  if((nPoints - 4) % 3 != 0)
  {
    fprintf(stderr, "Number of points expected to be 4, 7, 10, 13, 16..., got %d, %d subCurves\n", nPoints, nSubCurves);
    return false;
  }

  float x, y, z;

  double startDist = 0.0;

  Point<T> arr[4];

  //read the first point
  for(int j = 0; j < 4; ++j)
  {
    //read a point
    fscanf(csv, "%f, %f, %f ", &x, &y, &z);
    printf("Read point %f %f %f\n", x, y, z);
    arr[j] = Point<GLfloat>(x, y, z);
  }
  subCurves.push_back(SubCurve<T>(resolution, arr, startDist));
  startDist = subCurves.at(0).getEndDistance();

  for(int i = 1; i < nSubCurves; ++i)
  {
    arr[0] = arr[3]; //reuse the last point of the previous subcurve
    //read 3 points for each subcurve
    for(int j = 1; j < 4; ++j)
    {
      //read a point
      fscanf(csv, "%f, %f, %f ", &x, &y, &z);
      printf("Read point %f %f %f\n", x, y, z);
      arr[j] = Point<GLfloat>(x, y, z);
    }

    //add the new subcurve to the list
    subCurves.push_back(SubCurve<T>(resolution, arr, startDist));

    //set the distance offset from the last subcurve end
    startDist = subCurves.at(i).getEndDistance();
  }

  fclose(csv);
  
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

  int nSubCurves = subCurves.size();
  fprintf(fd, "%d\n", nSubCurves);

  Point<T> p[4];
  for(int i = 0; i < nSubCurves; ++i)
  {
    subCurves.at(i).getCtrlPoints(p);
    for(int j = 0; j < 4; ++j)
      fprintf(fd, "%f, %f, %f\n", p[j].getX(), p[j].getY(), p[j].getZ());
  }
  fclose(fd);
}


template <typename T>
bool Bezier<T>::valid()
{
  return subCurves.size() > 0;
}

//Helper function to compute the subCurve index and value of t necessary for
//a given percentage of the total distance along the curve
template <typename T>
std::pair<int, float> Bezier<T>::calcTforPercentDistance(float percentage)
{
  //don't allow negative percentages
  if(percentage < 0.f)
    return std::pair<int, float>(0, 0.f);
  //lock at 100%
  if(percentage >= 1.f)
    return std::pair<int, float>(subCurves.size() - 1, 1.f);

  //find the total distance of the composite curve
  //which is the last entry in the distance table of the last subcurve
  std::map<double, float>::iterator it = (*(subCurves.end() - 1)).distanceTable.end();
  it--; //back up, since end() is actually past the end
  double totalDist = (*it).first;
  double targetDist = totalDist * percentage;

  std::pair<double, float> lower, upper;
  //figure out which subcurve contains the target distance
  //start by guessing the same proportion as our percentage
  unsigned int subCurve = subCurves.size() * percentage; //will truncate
  std::map<double, float>* dtable;

  bool done = false;
  do
  {
    dtable = &subCurves.at(subCurve).distanceTable;
    it = dtable->upper_bound(targetDist);
    if(it != dtable->end())
    {
      //there is some entry higher than target
      it = dtable->lower_bound(targetDist);
      if(it != dtable->end())
      {
        //but there is the target or lower
        //the current subCurve contains our target
        done = true;
      }
      else if(subCurve > 0)
        subCurve--; //check the previous subcurve
      else
        fprintf(stderr, "No lower subcurve to seek target distance of %f\n", targetDist);
    }
    else
    {
      //no higher entry than target
      it = dtable->lower_bound(targetDist);
      if(it != dtable->end())
      {
        //but there is the target or lower
        //the current subCurve contains our target
        done = true;
      }
      else if(subCurve < subCurves.size() - 1)
        subCurve++; //check the next subcurve
      else
        fprintf(stderr, "No higher subcurve to seek target distance of %f\n", targetDist);
    }
  }
  while(!done);


  //find the closet pair of points in our distance table to our target and lerp
  dtable = &subCurves.at(subCurve).distanceTable;
  it = dtable->upper_bound(targetDist);
  if(it != dtable->begin())
    it--;
  lower = *it;
  upper = *(++it);

  double innerdist = upper.first - lower.first;
  double innertime = upper.second - lower.second;

  //What % of distance between our two samples is our target distance?
  double proportion = (targetDist - lower.first) / innerdist;

  //Our target time value will be the same proportion
  return std::pair<int, float>(subCurve, proportion * innertime + lower.second);
}

template class Bezier<float>; //Generates all template functions for use with float/GLfloat
template class SubCurve<float>;
