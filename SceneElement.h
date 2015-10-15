//SceneElement.h - A basic class representing anything that we might want to place and render
//  Mainly to be used as an abstract base class for inheritence and operations on any object
//Kevin Carbaugh

#pragma once
#include "Point.h"
#include "Orientation.h"
#include "Material.h"

class SceneElement
{
  public:
    SceneElement();
    SceneElement(const Point<float>& position, const Orientation<float>& ori, const Material& mat);

    Point<float> getPosition() const;
    Orientation<float> getOrientation() const;

    void setPosition(const Point<float>& pos);
    void setOrientation(const Orientation<float>& ori);
    void setMaterial(const Material& mat);

    void glSetupRender();
    void glEmplaceObject();
    void glOrientObject();

    //override with your drawing code, use glRender when you want to
    //place the object in worldspace with the above helper functions,
    //and then your render function will be called
    virtual void render();
    virtual void tick();

    Point<float> position;

  protected:
    Orientation<float> orientation;
    Material material;

    const static Vector<float> up;
};
