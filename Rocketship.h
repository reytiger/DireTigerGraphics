/*
 * Rocketship.h
 * Sesloc Avandara's vehicle of choice - a Rocketship
 * Defines an OpenGL object
 */

//render includes
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "BezierHero.h"
#include <cmath>

const double DEG2RAD = M_PI / 180;

class Rocketship : public BezierHero
{
  public:
    //ctor, detor
    Rocketship(float rocketScale); //constructor
    ~Rocketship();

    void draw();
    void tick(bool* keysDown);
    virtual void render();

    float theta; // The rocket's heading, in degrees
    void setThrustState(bool);

  private:
    void move(bool forward);

    void drawFin();
    void drawBody();
    void drawNose();
    void drawNozzle();
    void drawThrust();
	void drawName();

    //draw thrust only when going forward
    bool thrustState;
    //Animation rotations
    float finRotation;
    float nozzleRotation;
    float thrustCycle;
    //sizing to world coords
    float rocketScale;

    //scalars for movement
    static const float moveSpeed = 1.0;
    static const float rotateSpeed = 2;

    //Quadradic objects
    GLUquadricObj* bodyCylinder;
    GLUquadricObj* bodyBottomCap;
    GLUquadricObj* engineNozzle;
};
