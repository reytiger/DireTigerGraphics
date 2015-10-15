#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <math.h>
#include "Point.h"

class Wagon {
	float characterX, characterY, characterZ;   // character position in cartesian coordinates
	Point<GLfloat> location;
	float theta;                       // Character bearing in spherical coordinates
	float wheelTheta;                           // Wheel rotation variable
	
	Wagon();
	Wagon(float x, float y, float z, float th);
	
	void drawBench();
	void drawWheel();
	void drawWheels();
	void drawBox();
	void draw();
	
	void tick(bool* keysDown);

	// void rotate()? glRotatef(characterTheta*(180.0/M_PI), 0, 1, 0);
	
};