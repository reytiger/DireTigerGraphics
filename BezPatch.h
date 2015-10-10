//BezPatch.h - defines a Bezier patch
//Kevin Carbaugh

#include "Bezier.h"
#include "Point.h" //to be explicit, even though already in Bezier
#include "Vector.h"

template <typename T>
struct BezAxis
{
  Bezier<T> beziers[4];
};

template <typename T>
class SubPatch
{
  public:
    SubPatch(const std::vector<Point<T> >& ctrlPoints, int resolution);
    Bezier<T> evalAxis(float f = 0.f, bool u = true);

    struct BezAxis<T> uAxis;
    struct BezAxis<T> vAxis;
  private:
    void genAxes(const std::vector<Point<T> >& ctrlPoints);

    int resolution;
};

template <typename T>
class BezPatch
{
  public:
    BezPatch(int resolution); //ctor
    BezPatch(int resolution, const Point<T>& startPos); //ctor
    bool loadControlPoints(const char* const filename, float scaling);
    void render();

    Vector<T> getNormal(int subPatch, float u, float v);

    //getter
    int getResolution();

    //setter
    void setOrigin(const Point<T>& p);
    void setResolution(int res);
  private:
    void drawControlCage();
    

    std::vector<SubPatch<T> > subPatches; //patch data

    int resolution;
    int numPatches; //The number of 16 point patches
    Point<T> origin; //coordinate of the origin
    bool pointsLoaded;
};
