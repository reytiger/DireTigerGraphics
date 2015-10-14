//SceneElement.h - A basic class representing anything that we might want to place and render
//  Mainly to be used as an abstract base class for inheritence and operations on any object
//Kevin Carbaugh

#pragma once
#include "Point.h"
#include "Orientation.h"

class SceneElement
{
  public:
    SceneElement();
    SceneElement(const Point<float>& position, const Orientation<float>& ori);

    Point<float> getPosition() const;
    Orientation<float> getOrientation() const;

    void setPosition(const Point<float>& pos);
    void setOrientation(const Orientation<float>& ori);

    void glEmplaceObject();
    void glOrientObject();

    virtual void render(bool SelectionMode) = 0;

  protected:
    Point<float> position;
    Orientation<float> orientation;

    const static Vector<float> up;
};
