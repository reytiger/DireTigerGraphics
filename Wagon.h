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

class Wagon {
	float characterX, characterY, characterZ;   // character position in cartesian coordinates
	float characterTheta;                       // Character bearing in spherical coordinates
	float wheelTheta;                           // Wheel rotation variable
	
	Wagon();
	Wagon(float x, float y, float z, float cth);
	
	void drawBench();
	void drawWheel();
	void drawWheels();
	void drawBox();
	void drawWagon();
	
	void tick(bool* keysDown);
	
	void updateCharacterLeft();
	void updateCharacterRight();
	void updateCharacterForward();
	void updateCharacterBackward();
	// void rotate()? glRotatef(characterTheta*(180.0/M_PI), 0, 1, 0);
	
};