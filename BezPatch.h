//BezPatch.h - defines a Bezier patch
//Kevin Carbaugh

#include "Bezier.h"
#include "Point.h" //to be explicit, even though already in Bezier

template <typename T>
class BezPatch
{
  public:
    BezPatch(int resolution); //ctor
    BezPatch(int resolution, const Point<T>& startPos); //ctor
    bool loadControlPoints(const char* const filename, float scaling);
    void render();

    //setter
    void setOrigin(const Point<T>& p);
  private:
    void drawControlCage();

    int resolution;
    int numPatches; //The number of 16 point patches
    Point<T> origin; //coordinate of the origin
    std::vector<Point<T> > ctrlPoints; //The control points for the patches
    bool pointsLoaded;
};
