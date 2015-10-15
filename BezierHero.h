#pragma once
#include "Bezier.h"
#include "SceneElement.h"

class BezierHero : public SceneElement
{
  public:
    BezierHero();
    BezierHero(Bezier<float> curve, float stpSize, bool percntMode);

    bool loadCtrlPoints(FILE* fp);
    void setPercentageMode(bool mode);

    virtual void render();
    virtual void tick();

  private:
    Bezier<float> curve;
    float t;
    float stepSize;
    bool percentageMode;
      
};
