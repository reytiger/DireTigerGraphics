//BezPatch.h - defines a Bezier patch
//Kevin Carbaugh

#include "Bezier.h"

template <typename T>
class BezPatch
{
  public:
    BezPatch(int resolution); //ctor
    bool loadControlPoints(const char* const filename);
    void render();
  private:
    int resolution;
    int numPatches; //The number of 16 point patches
    std::vector<Point<T> > ctrlPoints; //The control points for the patches
};
