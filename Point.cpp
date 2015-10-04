//Point.cpp
//Kevin Carbagh

// OpenGL / GLUT Libraries we need
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Point.h"
#include <math.h>

//Scalar and point operations
template <typename T>
Point<T> operator-(const Point<T>& p)
{
  return Point<T>(-p.getX(), -p.getY(), -p.getZ());
}

template <typename T, typename S>
Point<T> operator*(const Point<T>& p, const S& factor)
{
  return Point<T>(p.getX() * factor, p.getY() * factor, p.getZ() * factor);
}

template <typename T, typename S>
Point<T> operator*(const S& factor, const Point<T>& p)
{
  return operator*(p, factor);
}

template <typename T>
Point<T> operator+(const Point<T>& a, const Point<T>& b)
{
  return Point<T>(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

template <typename T>
Point<T> operator-(const Point<T>& a, const Point<T>& b)
{
  return Point<T>(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

//GL helpers
template <typename T> void glVertexPoint(const Point<T>& p)
{
  //We don't know the template type, so promote to the most general (doubles)
  glVertex3d((GLdouble)p.getX(), (GLdouble)p.getY(), (GLdouble)p.getZ());
}

template <typename T> void glTranslatePoint(const Point<T>& p)
{
  glTranslated((GLdouble) p.getX(), (GLdouble) p.getY(), (GLdouble) p.getZ());
}

//Translating points
template <typename T>
Point<T> operator+(const Vector<T>& v, const Point<T>& p)
{
  return Point<T>(v.getX() + p.getX(), v.getY() + p.getY(), v.getZ() + p.getZ());
}

template <typename T>
Point<T> operator+(const Point<T>& p, const Vector<T>& v)
{
  return v + p;
}

template <typename T>
Point<T> operator-(const Point<T>& p, const Vector<T>& v)
{
  return Point<T>(p.getX() - v.getX(), p.getY() - v.getY(), p.getZ() - v.getZ());
}

template <typename T>
double distanceBetween(const Point<T>& a, const Point<T>& b)
{
  Point<T> diff = b - a;

  T x = diff.getX();
  T y = diff.getY();
  T z = diff.getZ();
  return sqrt(x * x + y * y + z * z);
}


//default ctor
template <typename T>
Point<T>::Point()
{
  reset();
}

//explicit ctor
template <typename T>
Point<T>::Point(T startX, T startY, T startZ) : x(startX), y(startY), z(startZ) {};

template <typename T>
Point<T>& Point<T>::operator+=(const Point<T>& rhs)
{
  *this = *this + rhs;
  return *this;
}

template <typename T>
Point<T>& Point<T>::operator-=(const Point<T>& rhs)
{
  *this = *this - rhs;
  return *this;
}

template <typename T>
Point<T>& Point<T>::operator+=(const Vector<T>& rhs)
{
  *this = *this + rhs;
  return *this;
}

template <typename T>
Point<T>& Point<T>::operator-=(const Vector<T>& rhs)
{
  *this = *this - rhs;
  return *this;
}

//inspectors
template <typename T>
const T& Point<T>::getX() const { return x; }

template <typename T>
const T& Point<T>::getY() const { return y; }

template <typename T>
const T& Point<T>::getZ() const { return z; }

//setters

template <typename T>
void Point<T>::setX(const T val) { x = val; }

template <typename T>
void Point<T>::setY(const T val) { y = val; }

template <typename T>
void Point<T>::setZ(const T val) { z = val; }

template <typename T>
void Point<T>::reset()
{
  x = y = z = 0.0;
}

//Template instances for avoiding linkage errors by generating specific functions
template class Point<float>;
template void glTranslatePoint<float>(const Point<float>&);
template void glVertexPoint<float>(const Point<float>&);
template Point<float> operator*<float, int>(const int&, const Point<float>&);
template Point<float> operator-<float>(const Point<float>&);
template Point<float> operator*<float, float>(const Point<float>&, const float&);
template double distanceBetween(const Point<float>& a, const Point<float>& b);
