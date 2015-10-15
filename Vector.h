//Vector.h - defines a 3D direction in cartesean coordinates
//Kevn Carbaugh

#ifndef VECTOR_H_INC
#define VECTOR_H_INC

template <typename T>
class Vector
{
  public:
    Vector();
    Vector(const T magX, const T magY, const T magZ);

    //class level
    static Vector<T> normalize(const Vector<T>& in);

    Vector<T> cross(const Vector<T>& other) const;
    T dot(const Vector<T>& other);
    double angleTo(const Vector<T>& other);
    Vector<T>& operator*=(const T& rhs);
    Vector<T> projectOnto(const Vector<T>& other);
    T magnitude() const;
    //instance level
    void normalize();
	
    void draw();
    void drawNormalized() const;

    //Component getters
    const T& getX() const;
    const T& getY() const;
    const T& getZ() const;

  private:
    T determinant(T a, T b, T c, T d) const;

    T x, y, z;
};


template <typename T>
struct Basis
{
  Vector<T> x;
  Vector<T> y;
  Vector<T> z;
};

//OpenGL helpers
template <typename T> void glNormalVector(Vector<T>& vec);
template <typename T> void glRotatefVector(const float, const Vector<T>& axis);
template <typename T> void glTranslatefVector(const Vector<T>& vec);
template <typename T> void glRotateToBasis(Vector<T>& z, const Vector<T>& translation); //orients along a given vector
template <typename T> void glRotateToBasis(const Basis<T>& b, const Vector<T>& translation); //orients to a given basis

//Operators
template <typename T> Vector<T> operator-(const Vector<T>& a);

template <typename T> Vector<T> operator+(const Vector<T>& a, const Vector<T>& b);
template <typename T> Vector<T> operator-(const Vector<T>& a, const Vector<T>& b);
template <typename T, typename S> Vector<T> operator*(const Vector<T>& a, const S& b);
template <typename T, typename S> Vector<T> operator*(const S& b, const Vector<T>& a);

#endif
