#ifndef POINT_H_INC
#define POINT_H_INC

#include "Vector.h"

template <typename T>
class Point
{
  public:
    Point();
    Point(T startX, T startY, T startZ);

    Point& operator+=(const Point& rhs);
    Point& operator-=(const Point& rhs);

    Point& operator+=(const Vector<T>& rhs);
    Point& operator-=(const Vector<T>& rhs);

    const T& getX() const;
    const T& getY() const;
    const T& getZ() const;

    void setX(const T);
    void setY(const T);
    void setZ(const T);

    void reset();

  private:
    T x, y, z;

};

//OpenGL helper functions
template <typename T> void glVertexPoint(const Point<T>& p);
template <typename T> void glTranslatePoint(const Point<T>& p);

//Unary operators
template <typename T> Point<T> operator-(const Point<T>& p); //negation

//Binary point operators
template <typename T, typename S>
Point<T> operator*(const Point<T>& p, const S& factor); //Point * point
template <typename T, typename S>
Point<T> operator*(const S& factor, const Point<T>& p); //float * point

template <typename T> Point<T> operator+(const Point<T>& a, const Point<T>& b); //Point + Point
template <typename T> Point<T> operator-(const Point<T>& a, const Point<T>& b); //Point - Point

//Vector addition
template <typename T> Point<T> operator+(const Vector<T>& v, const Point<T>& p); //Point + vector
template <typename T> Point<T> operator+(const Point<T>& p, const Vector<T>& v); 
//Vector subtraction
template <typename T> Point<T> operator-(const Point<T>& p, const Vector<T>& v); 


//Other util
template <typename T> double distanceBetween(const Point<T>& a, const Point<T>& b);

#endif
