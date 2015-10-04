//Bezier.h - Defines a class for working with cubic Bezier curves
//Kevin Carbaugh

#ifndef BEZIER_H_INC
#define BEZIER_H_INC

#include <vector>
#include <map>

#include "Point.h"
#include "Vector.h"

template <typename T>
class Bezier
{
  public:
    Bezier(int resolution);

    void render(bool selectionMode);

    static Point<T> evaluateCurve(Point<T> p0,
                                  Point<T> p1,
                                  Point<T> p2,
                                  Point<T> p3,
                                  float t);
    Point<T> getPercentageAlongCurve(float percentage);
    void setSelectedPoint(unsigned int index);
    void unsetSelectedPoint();

    void moveSelectedPoint(Vector<T> v);

    void toggleControlCageVisible();
    void toggleCurveVisible();

    bool loadControlPoints(const char* const filename);
    void saveControlPoints();
    bool valid();

  private:
    std::vector<Point<T> > controlPoints;
    std::map<double, float> distanceTable;

    Point<T>* origin; //the world coordinate to draw relative to

    void renderCurve(Point<T> p0,
                     Point<T> p1,
                     Point<T> p2,
                     Point<T> p3);
    void drawControlCage(int npoints);
    void drawControlPoints(bool selectionMode, int npoints);

    void populateDistanceTable();

    bool drawCage;
    bool drawCurve;

    int selectedPoint;
    int resolution;
};

#endif
