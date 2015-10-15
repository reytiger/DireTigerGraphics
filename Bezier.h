//Bezier.h - Defines a class for working with cubic Bezier curves
//Kevin Carbaugh

#pragma once
#include <vector>
#include <map>
#include <cstdio> //FILE* type

#include "Point.h"
#include "Vector.h"

//A bezier curve composed of 4 control points
template <typename T>
class SubCurve
{
  public:
    SubCurve();
    SubCurve(const int resolution, const Point<T> (&points)[4], const double startDist = 0.0);

    static Point<T> evaluateCurve(Point<T> p0,
                                  Point<T> p1,
                                  Point<T> p2,
                                  Point<T> p3,
                                  float t);
    Point<T> evaluateCurve(float t);
    Point<T> getPercentageAlongCurve(float percentage);

    static Vector<T> getTangent(Point<T> p0,
                                Point<T> p1,
                                Point<T> p2,
                                Point<T> p3,
                                float t);
    Vector<T> getTangent(float t);
    Vector<T> getTangentByPercentage(float percentage);

    double getEndDistance();

    void render(bool drawCtrlCage, bool drawCurve);


    //getters
    void getCtrlPoints(Point<T> (&arr)[4]);

    //setters
    void setOrigin(Point<T>* pt);

    std::map<double, float> distanceTable;

  private:
    float calcTforPercentDistance(float percentage);
    void populateDistanceTable(double startDist);
    void drawControlCage();
    void drawControlPoints();

    std::vector<Point<T> > controlPoints;
    Point<T>* origin; //the world coordinate to draw relative to

    int resolution;

};


//A composite curve made up of any number of subcurves
template <typename T>
class Bezier
{
  public:
    Bezier();
    Bezier(const int resolution);
    Bezier(const int resolution, const Point<T> (&points)[4]); 

    void render();

    Point<T> evaluateCurves(float t);
    Point<T> getPercentageAlongCurve(float percentage);

    Vector<T> getTangent(int subCurve, float t);
    Vector<T> getTangentByPercentage(float percentage);

    void toggleControlCageVisible();
    void toggleCurveVisible();

    bool loadControlPoints(const char* const filename);
    bool loadControlPoints(FILE* fd);
    void saveControlPoints();

    int getNumSubCurves();
    bool valid();

  private:
    std::vector<SubCurve<T> > subCurves;

    std::pair<int, float> calcTforPercentDistance(float percentage);

    bool drawCage;
    bool drawCurve;

    int resolution;
};
