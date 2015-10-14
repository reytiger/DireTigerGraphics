//class header
#include "Camera.h"

//implementation headers
#include <math.h>
#include <stdio.h>

#define DEG2RAD M_PI / 180

const float Camera::moveSpeed = 8.5f;

Camera::Camera(cameraMode startingMode, double startTheta, double startPhi, double dfo)
{
  currentMode = startingMode;

  objFollowing = false;
  objWatching = false;

  //initialize a bunch of member data
  phi = startPhi;
  theta = startTheta;
  distanceFromOrigin = dfo;

  worldPos = Point<GLfloat>(0.f, 0.f, 0.f);
  lookAtPoint = Point<GLfloat>(0.f, 0.f, 1.f);
  direction = Vector<GLfloat>(0.f, 0.f, 1.f);

  //Set our target property pointers to null
  originObj = watchPoint = 0;
  objHeading = 0;
}


void Camera::switchMode(cameraMode mode)
{
  if(currentMode != mode)
  {
    currentMode = mode;

    updateView();
  }
}


void Camera::updateView()
{
  //calculate the camera position and orientation
  recomputeCoordsAndDirection();

  //update our GLU camera's orientation
  gluLookAt(worldPos.getX(), worldPos.getY(), worldPos.getZ(),
            lookAtPoint.getX(), lookAtPoint.getY(), lookAtPoint.getZ(),
            0,  1,  0);

  //printf("CamX: %f, CamY: %f, CamZ: %f, LookX: %f, LookY: %f, LookZ: %f\n", worldPos.getX(), worldPos.getY(), worldPos.getZ(), lookAtPoint.getX(), lookAtPoint.getY(), lookAtPoint.getZ());
  //inform glut that we need to redraw when appropirate
  glutPostRedisplay();
}


void Camera::mouseRotate(int mouseDeltaX, double mouseDeltaY)
{
  switch(currentMode)
  {
  case FREE:
    //we have to invert the values here, otherwise the mouse will invert it's Y movement
    //in different modes, being a very inconsistent experience for the user
    phi = phiRef - mouseDeltaY * 0.001;
    theta = thetaRef + mouseDeltaX * 0.001;
    break;

  case ARCBALL:
    theta = thetaRef + mouseDeltaX * 0.001;
    phi = phiRef + mouseDeltaY * 0.001;

    break;
  //can't rotate in other modes
  default:
    return;
  }

  //make sure phi is in domain of spherical coordinates
  if(phi >= M_PI - 0.0001)
    phi = M_PI - 0.0002;
  if(phi <= 0)
    phi = 0.00001;

  //printf("Theta: %4.2f, Phi: %4.2f\n", theta, phi);
}


void Camera::objFollow(Point<GLfloat>* obj, float* objTheta)
{
  objFollowing = true;

  //Save our pointers in the class
  originObj = obj;
  objHeading = objTheta;
}


void Camera::objWatch(Point<GLfloat>* obj)
{
  objWatching = true;
  watchPoint = obj;
}


void Camera::unFollow()
{
  objFollowing = false;
  //re-zero our pointers
  originObj = 0;
}


void Camera::unWatch()
{
  objWatching = false;
  watchPoint = 0;
}


void Camera::move(bool forwards)
{
  Vector<GLfloat> movement = direction * moveSpeed;
  if(forwards)
    //Move our position forward along the direction vector
    worldPos += movement;
  else
    worldPos -= movement;
}


void Camera::saveReferenceFrame()
{
  //record our current params
  dfoRef = distanceFromOrigin;
  thetaRef = theta;
  phiRef = phi;
}


void Camera::resetPerspective(float aspectRatio)
{
  //update the projection matrix with the new window properties
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspectRatio, 0.1, 100000);
  glMatrixMode(GL_MODELVIEW);
}


void Camera::adjustDistFromOrigin(int mouseDeltaY)
{
  distanceFromOrigin = dfoRef + mouseDeltaY * 0.1;
  
  //limits
  if(distanceFromOrigin > 400)
    distanceFromOrigin = 400;
  if(distanceFromOrigin < 1)
    distanceFromOrigin = 1;

  //apply the changes immediately, if applicable
  if(currentMode == ARCBALL)
    updateView();
}


cameraMode Camera::getCurrentMode()
{
  return currentMode;
}


void Camera::recomputeCoordsAndDirection()
{
  //
  //Camera position, direciton
  //
  switch(currentMode)
  {
  case FREE:
    {
      lookStraight(true, false);
      break;
    }

  case ARCBALL:
    //Translate from our sphereical inputs to cartesian coords
    worldPos = Point<GLfloat>(distanceFromOrigin * sin(theta) * sin(phi),
               -distanceFromOrigin * cos(phi),
               -distanceFromOrigin * cos(theta) * sin(phi));

    //defaults for if we don't have an object to track
    lookAtPoint.reset();
    if(objFollowing)
    {
      worldPos += *originObj;
      lookAtPoint = *originObj;
    }
      
    break;

  case THIRD_PERSON:
    break;
    if(objWatching)
      //set our look at coords to that of the referenced obj
      lookAtPoint = *watchPoint;

  case FIRST_PERSON:
	worldPos = *originObj + Point<GLfloat>(0.0f, 0.0f, 13.0f);
	lookStraight (true, false);
    break;
  }
}

void Camera::lookStraight(bool useDTO, bool invert)
{
  //Use the internal distance to origin variable
  direction = Vector<GLfloat>(-sin(theta) * sin(phi), cos(phi), cos(theta) * sin(phi));
  if(useDTO)
    direction *= distanceFromOrigin;

  if(invert)
    //flip the camera from looking towards the outside the sphere
    //to looking inside the sphere. Necessary for free cam and arcball
    //to share a theta and phi value
    direction = -direction;

  direction = Vector<GLfloat>::normalize(direction);

  //Update where the camera will be looking
  lookAtPoint = worldPos + direction;
}


void Camera::updateObjectHeading()
{
  if(objHeading)
    theta = -DEG2RAD * (*objHeading);
}
