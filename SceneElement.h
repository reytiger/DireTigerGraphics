//SceneElement.h - A basic class representing anything that we might want to place and render
//  Mainly to be used as an abstract base class for inheritence and operations on any object
//Kevin Carbaugh

#pragma once
#include "Point.h"
#include "Vector.h"

class SceneElement
{
  public:
    SceneElement();
    SceneElement(const Point<float>& position, const Vector<float>& normal, const float heading);

    Point<float> getPosition() const;
    Vector<float> getNormal() const;
    float getHeading() const;

    void setPosition(const Point<float>& pos);
    void setNormal(const Vector<float>& norm);
    void setHeading(const float degrees);

    void glEmplaceObject();

    virtual void render(bool SelectionMode) = 0;

  protected:
    Point<float> position;
    Vector<float> normal; //where is up for this model?
    float theta; //rotation about normal to determine heading
};
