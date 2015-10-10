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

    Vector<T> cross(const Vector<T>& other);
    Vector<T>& operator*=(const T& rhs);
	
    void draw();
    void drawNormalized();

    //Component getters
    const T& getX() const;
    const T& getY() const;
    const T& getZ() const;

  private:
    T x, y, z;

    T determinant(T a, T b, T c, T d);
};

//OpenGL helpers
template <typename T> void glNormalVector(Vector<T>& vec);
//Operators
template <typename T> Vector<T> operator-(const Vector<T>& a);

template <typename T> Vector<T> operator+(const Vector<T>& a, const Vector<T>& b);
template <typename T> Vector<T> operator-(const Vector<T>& a, const Vector<T>& b);
template <typename T, typename S> Vector<T> operator*(const Vector<T>& a, const S& b);
template <typename T, typename S> Vector<T> operator*(const S& b, const Vector<T>& a);

#endif
