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

#define RAD2DEG 180/M_PI

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
void glTranslatefVector(const Vector<T>& vec)
{
  glTranslatef(vec.getX(), vec.getY(), vec.getZ());
}


template <typename T>
void glRotateToBasis(Vector<T>& z, const Vector<T>& translation)
{
  //cross with the world up vector to get the x axis
  Vector<T> x = z.cross(Vector<T>(0., 1., 0.));
  if(x.magnitude() == 0.) //if they are parallel
    x = Vector<T>(1., 0., 0.).cross(z);

  x.normalize();

  Vector<T> y = z.cross(x);
  Basis<T> b;
  b.x = x;
  b.y = y;
  b.z = z;

  glRotateToBasis(b, translation);

}


template <typename T>
void glRotateToBasis(const Basis<T>& b, const Vector<T>& translation)
{
  Vector<T> x = b.x;
  Vector<T> y = b.y;
  Vector<T> z = b.z;

  x.normalize();
  y.normalize();
  z.normalize();

  //we now have 3 basis vectors. Construct a rotation matrix
  GLfloat rotMat[16];
  
  //set w dimension
  for(int i = 0; i < 3; ++i)
  {
    rotMat[i * 4 + 3] = 0; //bottom left row
  }
  
  //set the 4th dimension column
  rotMat[12] = translation.getX() * y.getX();
  rotMat[13] = translation.getY() * y.getY();
  rotMat[14] = translation.getZ() * y.getZ();
  rotMat[15] = 1; //w

  //Set the vectors as the components of the matrix
  rotMat[0]  = x.getX();
  rotMat[1]  = x.getY();
  rotMat[2]  = x.getZ();

  rotMat[4]  = y.getX();
  rotMat[5]  = y.getY();
  rotMat[6]  = y.getZ();

  rotMat[8]  = z.getX();
  rotMat[9]  = z.getY();
  rotMat[10] = z.getZ();

  //apply the rotation
  glMultMatrixf(rotMat);
}


template <typename T>
void Vector<T>::draw() const
{
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
    glVertex3i(0, 0, 0); //start from the origin
    glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z); //and draw out to the terminal point
  glEnd();
  glEnable(GL_LIGHTING);
}


template <typename T>
void Vector<T>::drawNormalized() const
{
  normalize(*this).draw();
}

//ctors
template <typename T>
Vector<T>::Vector() {};

template <typename T>
Vector<T>::Vector(const T magX, const T magY, const T magZ) : x(magX), y(magY), z(magZ) {};


template <typename T>
Vector<T> Vector<T>::normalize(const Vector<T>& in)
{
  T mag = in.magnitude();
  return Vector<T>(in.x / mag, in.y / mag, in.z / mag);
}


//cross product
template <typename T>
Vector<T> Vector<T>::cross(const Vector<T>& other) const
{
  return Vector<T>(determinant(y, z, other.y, other.z),
                   -determinant(x, z, other.x, other.z),
                   determinant(x, y, other.x, other.y));
}


//dot product
template <typename T>
T Vector<T>::dot(const Vector<T>& other) const
{
  return x * other.x + y * other.y + z * other.z;
}


template <typename T>
double Vector<T>::angleTo(const Vector<T>& other) const
{
  return RAD2DEG * acos(dot(other) / (magnitude() * other.magnitude()));
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const T& rhs)
{
  *this = *this * rhs;
  return *this;
}


template <typename T>
Vector<T> Vector<T>::projectOnto(const Vector<T>& other)
{
  //projects this vector onto the given vector
  return other * ((dot(other)) / (other.magnitude() * other.magnitude()));
}


template <typename T>
T Vector<T>::magnitude() const
{
  return sqrt(x * x + y * y + z * z);
}


template <typename T>
void Vector<T>::normalize()
{
  T mag = magnitude();
  x /= mag;
  y /= mag;
  z /= mag;
}

//helper function for computing cross products
//Format is as follows:
//| a b |
//| c d |
template <typename T>
T Vector<T>::determinant(T a, T b, T c, T d) const
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
template void glTranslatefVector(const Vector<float>&);
template void glRotateToBasis(Vector<float>&, const Vector<float>&);
template void glRotateToBasis(const Basis<float>&, const Vector<float>&);
