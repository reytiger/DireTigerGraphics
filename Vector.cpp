#include "Vector.h"

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
Vector<T> operator-(const Vector<T>& a)
{
  return Vector<T>(-a.getX(), -a.getY(), -a.getZ());
}

template <typename T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b)
{
  return Vector<T>(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

template <typename T>
Vector<T> operator-(const Vector<T>& a, const Vector<T>& b)
{
  return Vector<T>(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

template <typename T, typename S>
Vector<T> operator*(const Vector<T>& a, const S& b)
{
  return Vector<T>(a.getX() * b, a.getY() * b, a.getZ() * b);
}

template <typename T, typename S>
Vector<T> operator*(const S& b, const Vector<T>& a)
{
  return a * b;
}


template <typename T>
void Vector<T>::draw()
{
  glColor3f(0.f, 1.f, 0.f);
  glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3f((GLfloat)x, (GLfloat))y, (GLfloat)z);
  glEnd();
}

//ctors
template <typename T>
Vector<T>::Vector() {};

template <typename T>
Vector<T>::Vector(const T magX, const T magY, const T magZ) : x(magX), y(magY), z(magZ) {};


template <typename T>
Vector<T> Vector<T>::normalize(const Vector& in)
{
  float mag = in.x * in.x + in.y * in.y + in.z * in.z;
  return Vector<T>(in.x / mag, in.y / mag, in.z / mag);
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const T& rhs)
{
  *this = *this * rhs;
  return *this;
}

template <typename T>
const T& Vector<T>::getX() const { return x; }

template <typename T>
const T& Vector<T>::getY() const { return y; }

template <typename T>
const T& Vector<T>::getZ() const { return z; }

//Explicitly instantiate some versios of the Vector template
//that we will use in order to avoid linkage errors

template class Vector<float>;
template Vector<float> operator-<float>(const Vector<float>&);
