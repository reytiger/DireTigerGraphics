//Orientation.h - Defines a type with pitch, yaw, and roll for orienting objects in 3D
//Kevin Carbaugh
#include "Vector.h"

template <typename T>
class Orientation
{
  public:
    Orientation();
    Orientation(T r, T p, T y);

    T getRoll() const;
    T getPitch() const;
    T getYaw() const;

    void setRoll(T r);
    void setPitch(T p);
    void setYaw(T y);

    void glOrient();

    const static Vector<float> rollAxis;
    const static Vector<float> pitchAxis;
    const static Vector<float> yawAxis;

  private:
    T roll;
    T pitch;
    T yaw;

    T bound(const T& val);
};
