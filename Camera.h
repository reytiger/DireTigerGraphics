//Camera.h
//Defines a set of basic camera modes for a 3D scene
//Kevin Carbaugh

#ifndef MODALCAMERA_H_INC
#define MODALCAMERA_H_INC

//GL inlucdes
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
#include "Vector.h"

typedef enum mode {FREE, ARCBALL, THIRD_PERSON, FIRST_PERSON} cameraMode;

class Camera
{
  public:
    Camera(cameraMode startingMode, double startTheta, double startPhi, double dfo);

    void switchMode(cameraMode mode);
    void updateView();

    //Setting where to point the view and position
    void mouseRotate(float mouseDeltaX, double mouseDeltaY);
    void objFollow(Point<GLfloat>* obj, float* objTheta);
    void objWatch(Point<GLfloat>* obj);
    void releaseObjLock();
    void unFollow();
    void unWatch();
    void move(bool forwards);
    
    void saveReferenceFrame();
    void resetPerspective(float aspectRatio);

    //getters/setters
    void adjustDistFromOrigin(int mouseDeltaY);
    cameraMode getCurrentMode();
    
  private:
    const static float moveSpeed;

    void recomputeCoordsAndDirection(); //sets internal coords and direction
    void lookStraight(bool useDTO, bool invert); //helper function to compute a unit vector along theta, phi
    void updateObjectHeading();

    cameraMode currentMode;

    //whether we have a free camera, or one that follows and object
    bool objFollowing;
    bool objWatching; //are we watching a fixed point or another object?
    
    //Params to GluLookAt
    Point<GLfloat> worldPos; //the camera's world coordinates
    Point<GLfloat> lookAtPoint; //where the camera is pointed

    double distanceFromOrigin, dfoRef; //used for arcball radius

    //Direction the camera is facing
    float theta, phi, thetaRef, phiRef;
    Vector<GLfloat> direction;

    //The coordinates we are translating our camera relative to
    Point<GLfloat>* originObj;
    float* objHeading;

    //The coordinates we are looking at in watch point mode
    Point<GLfloat>* watchPoint;
};

#endif
