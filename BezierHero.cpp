#include "BezierHero.h"

BezierHero::BezierHero() : SceneElement(), curve(), stepSize(0.01f), percentageMode(false) {};
BezierHero::BezierHero(Bezier<float> curve, float stpSize, bool percntMode) :
                       SceneElement(), curve(curve), stepSize(stpSize), percentageMode(percntMode) {};

bool BezierHero::loadCtrlPoints(FILE* fp)
{
  return curve.loadControlPoints(fp);
}

void BezierHero::setPercentageMode(bool mode)
{
  percentageMode = mode;
}

void BezierHero::render()
{
  if(curve.valid())
  {
    curve.render();
    if(percentageMode)
    {
      position = curve.getPercentageAlongCurve(t);
      //glTranslatePoint(position);
    }
    else
    {
      position = curve.evaluateCurves(t);
      //glTranslatePoint(position);
    }
  }
  glSetupRender();
}


void BezierHero::tick()
{
  t += stepSize;
  if(percentageMode)
  {
    //resets once above 100%
    if(t > 1.f)
      t--;
  }
  else if(t > curve.getNumSubCurves())
    t = 0.f;
}
