#include "Orientation.h"

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

//ctors
template <typename T>
Orientation<T>::Orientation() : roll(0.0), pitch(0.0), yaw(0.0) {};

template <typename T>
Orientation<T>::Orientation(T r, T p, T y) : roll(bound(r)), pitch(p), yaw(y) {};


template <typename T>
T Orientation<T>::getRoll() const { return roll; };

template <typename T>
T Orientation<T>::getPitch() const { return pitch; };

template <typename T>
T Orientation<T>::getYaw() const { return yaw; };

template <typename T>
void Orientation<T>::setRoll(T r)
{
  roll = bound(r);
}

template <typename T>
void Orientation<T>::setPitch(T p)
{
  pitch = bound(p);
}

template <typename T>
void Orientation<T>::setYaw(T y)
{
  yaw = bound(y);
}

//setup the openGL context to draw in
template <typename T>
void Orientation<T>::glOrient()
{
  glRotatefVector(yaw, yawAxis);
  glRotatefVector(pitch, pitchAxis);
  glRotatefVector(roll, rollAxis);
}

//enforce 0 - 359 degrees
template <typename T>
T Orientation<T>::bound(const T& val)
{
  T x = val;
  while(x < 0.)
    x += 360.;
  while(x >= 360.)
    x -= 360.;

  return x;
}

//constant statics
template <typename T>
const Vector<float> Orientation<T>::rollAxis = Vector<float>(0.f, 0.f, -1.f);

template <typename T>
const Vector<float> Orientation<T>::pitchAxis = Vector<float>(-1.f, 0.f, 0.f);

template <typename T>
const Vector<float> Orientation<T>::yawAxis = Vector<float>(0.f, 1.f, 0.f);

template class Orientation<float>;
