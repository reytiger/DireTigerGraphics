//Familiar.h - Defines the hero's companion/pet
//Kevin Carbaugh

#ifndef FAMILIAR_H_INC
#define FAMILIAR_H_INC

#include "Bezier.h"
#include "Point.h"

typedef float GLfloat;

class Familiar
{
  public:
    Familiar();
    void draw(bool selectionMode);
    void tick();
    void attachToObj(Point<GLfloat>* obj, float* rotation);

    Bezier<GLfloat> path;
  private:
    float percentage; //position along the path
    float speed; //speed of movement along path

    Point<GLfloat>* center;
    float* rot;
};

#endif
