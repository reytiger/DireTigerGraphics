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
SubPatch<T>::SubPatch(const std::vector<Point<T> >& ctrlPoints, int resolution) : resolution(resolution)
{
  if(ctrlPoints.size() != 16)
    fprintf(stderr, "ERROR: not enough control points specified for subpatch. Probably will crash here soon.\n");
  //generate the 4 control curves in either direction
  genAxes(ctrlPoints);
}

template <typename T>
void SubPatch<T>::genAxes(const std::vector<Point<T> >& ctrlPoints)
{
  //populates the two axis objects with 4 bezier curves
  //using the 16 control points
  Point<T> arr[4];
  for(int i = 0; i < 4; ++i)
  {
    for(int j = 0; j < 4; ++j)
      arr[j] = ctrlPoints.at(i * 4 + j);
    uAxis.beziers[i] = SubCurve<T>(resolution, arr);

    for(int j = 0; j < 4; ++j)
      arr[j] = ctrlPoints.at(i + 4 * j);
    vAxis.beziers[i] = SubCurve<T>(resolution, arr);
  }
}


//Generates a Bezier curve from the control points
//in either the u or v axis, at time = t
template <typename T>
SubCurve<T> SubPatch<T>::evalAxis(float t, bool u)
{
  Point<T> arr[4];
  for(int i = 0; i < 4; ++i)
  {
    if(u) //producing a curve along V
      arr[i] = uAxis.beziers[i].evaluateCurve(t);
    else //producing a u curve
      arr[i] = vAxis.beziers[i].evaluateCurve(t);
  }
  return SubCurve<T>(resolution, arr);
}

template class SubPatch<float>; //special instance of the template

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
  bool status = loadControlPoints(csv, scaling);
  fclose(csv);
  return status;
}


template <typename T>
bool BezPatch<T>::loadControlPoints(FILE* fp, float scaling)
{
  if(fscanf(fp, "%d ", &numPatches) != 1)
  {
    fprintf(stderr, "Unable to read number of BezPatches in file!\n");
    fclose(fp);
    return false;
  }

  float x, y, z;
  std::vector<Point<T> > ctrlPoints;
  for(int patch = 0; patch < numPatches; ++patch)
  {
    //Read the first patch
    for(int i = 0; i < 16; ++i)
    {
      //read a point
      int stat = fscanf(fp, "%f, %f, %f ", &x, &y, &z);
      if(stat != 3)
        fprintf(stderr, "Warning: didn't read complete coordinate for BezPatch\n");

      printf("Read point %f %f %f\n", x, y, z);
      ctrlPoints.push_back(Point<GLfloat>(x * scaling, y * scaling, z * scaling));
    }
    subPatches.push_back(SubPatch<T>(ctrlPoints, resolution));
    ctrlPoints.clear();
  }

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
    SubPatch<T>& s = subPatches.at(p);
    for(int i = 0; i < 4; ++i)
    {
      //draw the u curves so we can see control cages
      s.uAxis.beziers[i].render(true, false);
      s.vAxis.beziers[i].render(true, false);
    }

    float u = 0.f;
    float v = 0.f;
    const float step = 1.f / resolution;

    //TODO convert to a materials class
    //setup openGL material for the curve
    float diffcol[4] = {0.530f, 0.886f, 0.275f, 1.f};
    float speccol[4] = {0.8f, 0.8f, 0.8f, 1.f};
    float ambcol[4] = {0.2f, 0.2f, 0.2f, 1.f};
    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffcol);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, speccol);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambcol);

    //iterate through all subpatches
    //iterate through the u axis at the given resolution
    for(int i = 0; i < resolution; ++i)
    {
      //inverse of resolution will tell us how what % of the curve
      //length is each quad's dimensions
      u = i * step; 

      //calculate the control points for two v beziers
      //which will determine points along each edge of the quad
      SubCurve<T> vBezNear = s.evalAxis(u);
      SubCurve<T> vBezFar = s.evalAxis(u + step);

      //printf("u: %3.2f, u + stp: %3.2f, step: %3.2f, res: %d\n", u, u + step, step, resolution);
      glPushMatrix();
        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= resolution; ++j)
        {
          v = j * step;
          //printf("\tv: %3.2f\n", v);
          //Draw a quad on the surface, a pair of vertices at a time
          
          //set this vertex's normal
          Vector<T> norm = getNormal(p, u, v);
          glNormalVector(norm);
          //Evaluate a point on the surface
          glVertexPoint(vBezFar.evaluateCurve(v));

          norm = getNormal(p, u + step, v);
          glNormalVector(norm);
          glVertexPoint(vBezNear.evaluateCurve(v));
        }
        glEnd();
      glPopMatrix();


      glPushMatrix();
      glTranslatef(0.f, 0.2f, 0.f); //slightly offset so we can see
      //iterate through v again, drawing the computed tangent
      for(int j = 0; j <= resolution; ++j)
      {
        v = j * step;


        glPushMatrix();
          glTranslatePoint(vBezNear.getPercentageAlongCurve(v));
          //green u tangent
          glColor3f(0.f, 1.f, 0.f);
          //draw the tangent that is computed at this point
          (vBezNear.getTangentByPercentage(v)).drawNormalized();
          
          //blue v tangent
          glColor3f(0.f, 0.f, 1.f);
          (s.evalAxis(v, false).getTangentByPercentage(u)).drawNormalized();
          glColor3f(1.f, 0.f, 0.f);
          //draw the normal
          getNormal(p, u, v).draw();
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.f, 1.f, 0.f);
        glTranslatePoint(vBezFar.getPercentageAlongCurve(v));
        //draw the tangent that is computed at this point
          (vBezFar.getTangentByPercentage(v)).drawNormalized();
        glPopMatrix();
      }
      glPopMatrix();
    }
  }
}


template <typename T>
Vector<T> BezPatch<T>::getNormal(int subPatch, float u, float v)
{
  SubPatch<T>& s = subPatches.at(subPatch);
  
  //generate the two perpendicular curves
  SubCurve<T> vBez = s.evalAxis(u, true);
  SubCurve<T> uBez = s.evalAxis(v, false);

  return Vector<T>::normalize(vBez.getTangent(v).cross(uBez.getTangent(u)));
}

template <typename T>
Vector<T> BezPatch<T>::getTangent(int subPatch, float u, float v, bool uAxis)
{
  SubPatch<T>& s = subPatches.at(subPatch);

  if(uAxis)
    return s.evalAxis(v, false).getTangent(u);
  else
    return s.evalAxis(u, true).getTangent(v);
}

//evaluates a point on the patch in uv coordinates
template <typename T>
Point<T> BezPatch<T>::getCoord(int subPatch, float u, float v)
{
  if(!pointsLoaded) return Point<T>(0., 0., 0.);

  SubPatch<T>& s = subPatches.at(subPatch);
  SubCurve<T> vBez = s.evalAxis(u, true);
  return vBez.getPercentageAlongCurve(v);
}


template <typename T>
Basis<T> BezPatch<T>::getBasis(int subPatch, float u, float v)
{
  struct Basis<T> b;
  SubPatch<T>& s = subPatches.at(subPatch);

  //get the two tangent vectors of the curves
  b.x = Vector<T>::normalize(s.evalAxis(u, true).getTangent(v));
  b.z = Vector<T>::normalize(s.evalAxis(v, false).getTangent(u));

  //calculate a normal
  b.y = Vector<T>::normalize((b.x).cross(b.z));

  //make a new vector orothonormal to the u derivative and the normal
  b.x = Vector<T>::normalize((b.y).cross(b.z));

  return b;
}


template <typename T>
void BezPatch<T>::glOrientToSurface(int subPatch, float u, float v)
{
  //translate onto to the surface
  glTranslatePoint(getCoord(subPatch, u, v) + origin);

  //axis so object's up is along the normal
  Vector<T> norm = getNormal(subPatch, u, v);
  const Vector<T> yRef(0.0, 1.0, 0.0);
  Vector<T> axis = yRef.cross(norm);

  //calc axis of rotation so the object's up is along the normal
  const Vector<T> zRef(0.0, 0.0, 1.0);
  Vector<T> axis2 = zRef.cross(axis);


  //draw the world XYZ vectors
  glPushMatrix();
    glLineWidth(4.0f);
    //+X is red
    glColor3ub(255, 0, 0);
    Vector<T>(1.0, 0.0, 0.0).draw();

    //+Y is green
    glColor3ub(0, 255, 0);
    yRef.draw();

    //+Z is blue
    glColor3ub(0, 0, 255);
    zRef.draw();

    //the normal is grey
    glColor3ub(145, 145, 145);
    norm.draw();

    glLineWidth(1.0f);
  glPopMatrix();

  //apply rotations
  glRotatefVector(norm.angleTo(yRef), axis);
//  glRotatefVector(axis.angleTo(zRef), axis2);

  //draw the transformed axes
  glPushMatrix();
    glLineWidth(2.0f);
    //+X is yellow
    glColor3ub(226, 229, 39);
    Vector<T>(1.0, 0.0, 0.0).draw();

    //+Y is white
    glColor3f(0.8f, 0.8f, 0.8f);
    yRef.draw();

    //+Z is purplse
    glColor3ub(140, 37, 131);
    zRef.draw();

    //First axis is pink
    glColor3ub(216, 147, 183);
    axis *= 4;
    axis.draw();

    //Second axis is cyan
    glColor3ub(22, 122, 120);
    axis2 *= 4;
    axis2.draw();

    glLineWidth(1.0f);
  glPopMatrix();
  
  //and now the object should draw itself
}


template <typename T>
int BezPatch<T>::getResolution()
{
  return resolution;
}


template <typename T>
void BezPatch<T>::setOrigin(const Point<T>& p)
{
  origin = p;
}


template <typename T>
int BezPatch<T>::getNSubPatches()
{
  return numPatches;
}


template <typename T>
void BezPatch<T>::setResolution(int res)
{
  //don't allow a resolution of less than 1x1 quad
  if(res < 1)
    res = 1;
  resolution = res;
}


template class BezPatch<float>; //special instance this template
