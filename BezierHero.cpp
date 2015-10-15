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
  glEmplaceObject();
  if(curve.valid())
  {
    curve.render();
    //glRender();
    if(percentageMode)
      glTranslatePoint(curve.getPercentageAlongCurve(t));
    else
      glTranslatePoint(curve.evaluateCurves(t));
  }
  glApplyMaterial();
  //debug cube
  glutSolidCube(1);
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
