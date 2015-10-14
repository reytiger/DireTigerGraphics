#include "Vector.h"
#include <cmath>

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
void glNormalVector(Vector<T>& vec)
{
  glNormal3f((GLfloat) vec.getX(), (GLfloat) vec.getY(), (GLfloat) vec.getZ());
}


template <typename T>
void glRotatefVector(const float degrees, const Vector<T>& axis)
{
  glRotatef(degrees, axis.getX(), axis.getY(), axis.getZ());
}


template <typename T>
void Vector<T>::draw()
{
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
    glVertex3i(0, 0, 0); //start from the origin
    glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z); //and draw out to the terminal point
  glEnd();
  glEnable(GL_LIGHTING);
}


template <typename T>
void Vector<T>::drawNormalized()
{
  normalize(*this).draw();
}

//ctors
template <typename T>
Vector<T>::Vector() {};

template <typename T>
Vector<T>::Vector(const T magX, const T magY, const T magZ) : x(magX), y(magY), z(magZ) {};


template <typename T>
Vector<T> Vector<T>::normalize(const Vector& in)
{
  T mag = in.magnitude();
  return Vector<T>(in.x / mag, in.y / mag, in.z / mag);
}


//cross product
template <typename T>
Vector<T> Vector<T>::cross(const Vector<T>& other)
{
  return Vector<T>(determinant(y, z, other.y, other.z),
                   -determinant(x, z, other.x, other.z),
                   determinant(x, y, other.x, other.y));
}


//dot product
template <typename T>
T Vector<T>::dot(const Vector<T>& other)
{
  return x * other.x + y * other.y + z * other.z;
}


template <typename T>
double Vector<T>::angleTo(const Vector<T>& other)
{
  return acos(this->dot(other) / (this->magnitude() * other.magnitude()));
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const T& rhs)
{
  *this = *this * rhs;
  return *this;
}


template <typename T>
T Vector<T>::magnitude() const
{
  return sqrt(x * x + y * y + z * z);
}

//helper function for computing cross products
//Format is as follows:
//| a b |
//| c d |
template <typename T>
T Vector<T>::determinant(T a, T b, T c, T d)
{
  return a * d - b * c;
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
template void glNormalVector(Vector<float>&);
template void glRotatefVector(const float, const Vector<float>&);
