//Light.h - defines an openGL light for use in scenes
#include "Point.h"
#include "Vector.h"
#include "Color.h"

#define NUM_LIGHTS 8

enum LightType { DIRECTED, OMNI, SPOT };

class Light
{
  public:
    Light();
    Light(LightType type, const Point<float>& pos, const Color& diff, const Color& spec, const Color& ambi, const Vector<float> dir = Vector<float>(0.f, 0.f, -1.f));
    ~Light();

    void setColorDiff(const Color& col);
    void setColorAbmi(const Color& col);
    void setColorSpec(const Color& col);

    void setPosition(const Point<float>& pos);
    void setDirection(const Vector<float>& direction);
    void setSpotParams(const Vector<float>& direction, float angle, float falloff);
    void changeType(LightType newType);

  private:
    void setGLLightProperties();
    void assignGLLightNum();
    void updateColor();
    void updatePosition();
    void updateDirection();
    void applySpotParams();


    LightType type;
    Point<float> position;
    Vector<float> direction;
    Color diffuseCol;
    Color specularCol;
    Color ambientCol;
    float angleCutoff;
    float distanceFalloff;

    int glLightNum;

    static const int numLights;
    static bool lightNums[NUM_LIGHTS];
};
