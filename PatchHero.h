//PatchHero.h - the hero that moves along our bezier patch

#pragma once
#include "SceneElement.h"
#include "BezPatch.h"

class PatchHero : public SceneElement
{
  public:
    PatchHero(BezPatch<float>& patch, const float u, const float v);

    void setU(const float newU);
    void setV(const float newV);

    void incU(const float amt);
    void incV(const float amt);

    void render(bool selectionMode);
  private:
    BezPatch<float>& myPatch;
    float u;
    float v;
};
