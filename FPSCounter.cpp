#include "FPSCounter.h"
#include <string.h>
#include <cstdio>

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#define BUFFLEN 11
FPSCounter::FPSCounter() : nSamples(0), timer(0.)
{
  if(time(&lastTime) == -1)
    fprintf(stderr, "Couldn't get the current time!\n");
  lastUpdate = lastTime;
};

void FPSCounter::onRender(size_t winWidth, size_t winHeight)
{
  nSamples++;

  time_t curTime;
  if(time(&curTime) == -1)
  {
    fprintf(stderr, "Couldn't get the current time!\n");
    return;
  }
  //accumlate time since the last frame
  timer += difftime(curTime, lastTime);
  lastTime = curTime;
  if(difftime(curTime, lastUpdate) > 1.f) //update the display every second
  {
    avgFps = nSamples / timer;
    lastUpdate = curTime;
  }
  drawFPSCount(winWidth, winHeight);
  if(timer > 3.f) //average the drawing over the last 3 seconds
  {
    //nSamples = the number of frames renderd in the last second
    nSamples = 0;
    timer = 0.;
  }
}

void FPSCounter::drawFPSCount(size_t winWidth, size_t winHeight)
{
  //setup for 2D drawing
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix(); //preserve current projection matrix
    glLoadIdentity();

    //new projection matrix
    gluOrtho2D(0., winWidth, 0., winHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    {
      glLoadIdentity();
      glColor3f(1.f, 1.f, 1.f);

      //Formulate the display string
      char str[BUFFLEN];
      strncpy(str, "FPS: ", 5);
      sprintf(str + 5, "%4.2f", avgFps);
      str[BUFFLEN - 1] = '\0'; //terminate the string

      //set the rendering position
      glRasterPos2i(0, winHeight - 10);
      //display the string
      for(int i = 0; i < BUFFLEN; ++i)
      {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
      }
    }
    glPopMatrix();

    //restore the projection matrix
    glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  //mack to modelview to be safe
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}
