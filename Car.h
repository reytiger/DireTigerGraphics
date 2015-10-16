#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Point.h"
#include "Light.h"
#include <math.h>
#include "SceneElement.h"
#include "PatchHero.h"
const double DEGREE2RAD = M_PI / 180;

class Car : public SceneElement
{
  public:
    //ctor, detor
    Car(double startX, double startY, double startZ); //constructor
    ~Car();
    

    void draw();
    void tick(bool* keysDown);

    Point<GLfloat> location; //coordinates in scene
    float theta; // The rocket's heading, in degrees
	float value, updown;
	int wheelAngle;
	
	// Headlights
	Light* leftHeadlight;
	Light* rightHeadlight;

    void drawHero();
	void drawWheels();
	void drawName();
	
  private:
    void move(bool forward);

	
    static const float moveSpeed = 1.0;
    static const float rotateSpeed = 2;

};
