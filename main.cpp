/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: lab03
 *  File: main.cpp
 *
 *	Author: Dr. Jeffrey Paone - Fall 2015
 *
 *  Description:
 *      Contains the base code for 3D Bezier Curve visualizer.
 *
 */

// HEADERS /////////////////////////////////////////////////////////////////////

// OpenGL / GLUT Libraries we need
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

// C Libraries we need
#include <time.h>			// for time() to seed our RNG
#include <stdio.h>			// allow to print to terminal
#include <stdlib.h>			// access to rand() and exit()
#include <math.h>			// for cosf(), sinf(), etc.
#include <string.h>


// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream	
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)
#include <iostream>
using namespace std;


// Headers We've Written
#include "Point.h"
#include "Camera.h"
#include "BezPatch.h"
#include "Familiar.h"
#include "Rocketship.h"
#include "Car.h"
#include "Wagon.h"
#include "Light.h"
#include "FPSCounter.h"
#include "PatchHero.h"


// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;
int updown = 1;
bool heroArc1 = false, heroArc2 = false, heroArc3 = false;


GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

Camera cam(ARCBALL, 45.0, 2 * M_PI / 3, 30);
Light* mainLight = NULL;
FPSCounter fpsCounter;
Camera cam2(ARCBALL, 0.0, 2 * M_PI / 3, 30);
Camera cam3(ARCBALL, 0.0, 2 * M_PI / 3, 30);
GLint menuId;				    // handle for our menu

bool keysDown[256];
bool ctrlState;

//Scene objects
GLuint environmentDL;
Rocketship mandrake(0.3);
Car vehicle(15, 0, 15);
Wagon wagon(25.0f, 0.0f, 25.0f);
Familiar myFamiliar;
GLUquadricObj* treeTrunk;
PatchHero* pHero;

BezPatch<GLfloat> testPatch(6, Point<GLfloat>(0.f, 40.f, 0.f));

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
    return rand() / (float)RAND_MAX;
}

// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
    /*
     *	We will get to why we need to do this when we talk about lighting,
     *	but for now whenever we want to draw something with an OpenGL
     *	Primitive - like a line, quad, point - we need to disable lighting
     *	and then reenable it for use with the GLUT 3D Primitives.
     */
    glDisable( GL_LIGHTING );

    glLineWidth(2.0f);	// make our grid lines a little thicker so they are easier to see
    glBegin(GL_LINES);
    int gridSize = 200 / 2;
    //Draw the grid in white
    glColor3ub(255, 255, 255);
    for(int i = -gridSize; i <= gridSize; ++i)
    {
      //draw a line along the x axis
      glVertex3f(i, 0, -gridSize);
      glVertex3f(i, 0, gridSize);

      //draw a line along the z axis
      glVertex3f(gridSize, 0, i);
      glVertex3f(-gridSize, 0, i);
    }
    glEnd();
    glLineWidth(1.0f);

    glEnable( GL_LIGHTING );
}

void drawTree()
{
  glPushMatrix();
  {
    glRotatef(90, -1, 0, 0);
    //a nice brown color
    glColor3ub(158, 106, 28);
    gluCylinder(treeTrunk, 1, 1, 10, 6, 1);

    //green for the top
    glColor3ub(29, 150, 18);
    //Put ourselves on top of our trunk
    glTranslatef(0, 0, 10);
    glutSolidCone(5, 15, 6, 4);

  }
  glPopMatrix();
}


// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using GLUT 3D Primitives
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
  //expanded 100x100 grid
  int gridSize = 200 / 2;
  for(int x = -gridSize; x <= gridSize; ++x)
  {
    for(int z = -gridSize; z <= gridSize; ++z)
    {
      //Determines the density of buildings on our grid
      if(getRand() < 0.004)
      {
        glPushMatrix();
          //choose a random color
          glColor3f(getRand(), getRand(), getRand());

          //scale the cube by some random factor to vary the heights
          float cubeHeight = getRand() * 30 + 4;
          float cubeWidth = getRand() * 9 + 2;

          //place the cube on the grid so the bottom is on the xz plane
          glTranslatef(x, cubeHeight / 2, z);

          //and rotate to give the scene a bit more variety
          glRotatef(getRand() * 180, 0, 1, 0);
          //glScalef(1, cubeHeight, 1);
          glScalef(cubeWidth, cubeHeight, cubeWidth);

          //draw a cube of a random size
          glutSolidCube(1);
        glPopMatrix();

        //now translate over additonally so we help prevent the next building
        //from clipping into the one we just placed
        //z += (2 * cubeWidth) - 1; //-1 since the for loop is about to increment z anyway
      }
      else if(getRand() < 0.002)
      {
        glPushMatrix();
          //place in worldspace
          glTranslatef(x, 0, z);
          
          //scale randomly
          glScalef(getRand() * .9 + 0.5, getRand() * 0.7 + 0.4, getRand() * 0.5 + 0.3);
          drawTree();
        glPopMatrix();
      }
    }
  }
}

// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple 
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
  //request a single handle
  environmentDL = glGenLists(1);
  
  glNewList(environmentDL, GL_COMPILE);
  //Put the city and the grid into the display list
    glPushMatrix(); {
      drawGrid();
    }; glPopMatrix();

    glPushMatrix(); {
      drawCity();
    }; glPopMatrix();

  glEndList();
    
}
// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);
	//glViewport(w - 100, h-100, w, h);

    cam.resetPerspective(aspectRatio);
	cam2.resetPerspective(aspectRatio);
	cam3.resetPerspective(aspectRatio);
}


// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
    //update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;

    //update the left and right mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON)
    {
        leftMouseButton = state;
		cam.saveReferenceFrame();
        cam2.saveReferenceFrame();
		cam3.saveReferenceFrame();
    }

    //allow passive mouse motion to know if control is held
    ctrlState = glutGetModifiers() == GLUT_ACTIVE_CTRL;
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadius based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y)
{
  if(leftMouseButton == GLUT_DOWN)
  {
    if(ctrlState)
      cam2.adjustDistFromOrigin(y - mouseY);
	if(cam2.getCurrentMode() != FIRST_PERSON){
      cam2.mouseRotate(x - mouseX, mouseY - y);
	}
  }
  cam2.updateView();

  /*mouseX = x;
  mouseY = y;*/
}



// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()  {
    glEnable(GL_DEPTH_TEST);

    //construct our primary light source
    Color diffCol(0.4f, 0.4f, 0.4f, 1.f);
    Color ambientCol(0.1f, 0.1f, 0.1f, 1.0f);
    Color specularCol(0.2f, 0.2f, 0.2f, 1.f);
    Point<float> pos(150.f, 70.f, 10.f);

    glEnable( GL_LIGHTING );
    mainLight = new Light(OMNI, pos, diffCol, specularCol, ambientCol);
	
    glShadeModel(GL_SMOOTH);

    treeTrunk = gluNewQuadric();
    generateEnvironmentDL();

    pHero = new PatchHero(testPatch, 0, 0.5f, 0.5f);
    //attach our familiar to the ship
    myFamiliar.attachToObj(&mandrake.position, &mandrake.theta);

    //offset the rocketship
    mandrake.setPosition(Point<float>(0.f, 10.f, 0.f));

    //TODO
    //set materials
    GLfloat diffuse[4] = {0.55f, 0.25f, 0.25f, 1.f};
    GLfloat specular[4] = {0.003f, 0.003f, 0.003f, 1.f};
    GLfloat ambient[4] = {0.f, 0.f, 0.f, 1.f};

	
    /*GLfloat diffuse2[4] = {0.54f, 0.89f, 0.63f, 1.f};
    GLfloat specular2[4] = {0.316228f, 0.7316228f, 0.316228f, 1.f};
    GLfloat ambient2[4] = {0.135f, 0.2225f, 0.1575f, 1.f};*/
    mandrake.setMaterial(Material(diffuse, specular, ambient, 0.25f));
    //vehicle.setMaterial(Material(diffuse2, specular2, ambient2, .25f));

    if(!testPatch.loadControlPoints("testPatch.csv", 10.f))
      fprintf(stderr, "Could not load test bezier patch data from file\n");

    srand( time(NULL) );	// seed our random number generator

}


void drawStuff(){
	
	glPushMatrix();
    testPatch.render();
  glPopMatrix();

	glPushMatrix();
    pHero->render();
  glPopMatrix();
	
	glPushMatrix();
	glCallList(environmentDL);
	glPopMatrix();
	
	mandrake.render();
	vehicle.draw();
	wagon.draw();
	myFamiliar.draw(false);
	
}


// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene2(void)  {
    // clear the render buffer
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.updateView();

    drawStuff();

    //2D drawing
    fpsCounter.onRender(windowWidth, windowHeight);
    //push the back buffer to the screen
    glutSwapBuffers();
}


void renderScene(){
    // clear the render buffer
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 0.1, 100000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if(cam.getCurrentMode() == FIRST_PERSON){
	  cam.mouseRotate(vehicle.theta, 0);
	}
	if(cam2.getCurrentMode() == FIRST_PERSON){
	  cam2.mouseRotate(vehicle.theta, 0);
	}
	cam2.updateView();
	drawStuff();
	
	if (heroArc1 == true){
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(windowWidth/2, windowHeight/2, windowWidth/2, windowHeight/2);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS); {
			glColor3f(0, 0, 0);
			glVertex2f(0, 0);
			glVertex2f(0, 1.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(1.0f, 0);
		} glEnd();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45.0, aspectRatio, 0.1, 100000);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		if(cam.getCurrentMode() == FIRST_PERSON){
	      cam.mouseRotate(vehicle.theta, 0);
		}
		cam.updateView();
		drawStuff();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopMatrix();
	}
	
	else if (heroArc2 == true){
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(windowWidth/2, windowHeight/2, windowWidth/2, windowHeight/2);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS); {
			glColor3f(0, 0, 0);
			glVertex2f(0, 0);
			glVertex2f(0, 1.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(1.0f, 0);
		} glEnd();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45.0, aspectRatio, 0.1, 100000);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		if(cam2.getCurrentMode() == FIRST_PERSON){
	      cam2.mouseRotate(vehicle.theta, 0);
		}
		cam2.updateView();
		drawStuff();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopMatrix();
	}
	
	else if (heroArc3 == true){
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(windowWidth/2, windowHeight/2, windowWidth/2, windowHeight/2);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS); {
			glColor3f(0, 0, 0);
			glVertex2f(0, 0);
			glVertex2f(0, 1.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(1.0f, 0);
		} glEnd();
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45.0, aspectRatio, 0.1, 100000);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		cam3.updateView();
		drawStuff();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopMatrix();
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glutSwapBuffers();
	
}


// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown( unsigned char key, int x, int y ) {
    if( key == 'q' || key == 'Q' || key == 27 )
    {
      gluDeleteQuadric(treeTrunk);
      exit(0);
    }

    //change resolution of bezier patch
    if(key == '[')
      testPatch.setResolution(testPatch.getResolution() - 1);
    else if(key == ']')
      testPatch.setResolution(testPatch.getResolution() + 1);



    keysDown[key] = true;
}

// normalKeysUp() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user releases
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysUp( unsigned char key, int x, int y )
{
  keysDown[key] = false;
}

// myTimer() ////////////////////////////////////////////////////////////////////
//
//  GLUT timer callback; gets called when a timer expires
//
////////////////////////////////////////////////////////////////////////////////
void myTimer( int value )
{
  //animate the ship
  //mandrake.tick(keysDown);
  mandrake.BezierHero::tick();

  vehicle.tick(keysDown);
  wagon.timetick(keysDown);
  
  myFamiliar.tick();

  //camera mode changes
  //if(keysDown['1'] && cam.getCurrentMode() != FREE)
  //  cam.switchMode(FREE);
  //else if(keysDown['2'] && cam.getCurrentMode() != ARCBALL)
  //  cam.switchMode(ARCBALL);

  //camera movement
  if(cam.getCurrentMode() == FREE)
  {
    if(keysDown['z'] || keysDown['Z'])
      cam.move(true);
    else if(keysDown['x'] || keysDown['X'])
      cam.move(false);
  }

  //move hero around on patch
  //between patches
  if(keysDown['u'])
    pHero->prevSubPatch();
  else if(keysDown['o'])
    pHero->nextSubPatch();

  //V axis
  if(keysDown['i'])
    pHero->incV(0.01f);
  else if(keysDown['k'])
    pHero->incV(-0.01f);

  //U axis
  if(keysDown['l'])
    pHero->incU(0.01f);
  else if(keysDown['j'])
    pHero->incU(-0.01f);

  //tick events
  pHero->tick();

  //update the camera's view and ask for a redraw
  cam.updateView();
  // register a new timer callback
  glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

// myMenu() /////////////////////////////////////////////////////////////////////
//
//  Handles our Menu events
//
////////////////////////////////////////////////////////////////////////////////
void myMenu( int value ) {
	// TODO #02: handle our menu options
  switch(value)
  {
  case 0:
    myFamiliar.path.toggleControlCageVisible();
    break;
  case 1:
    myFamiliar.path.toggleCurveVisible();
    break;
  case 2:
    exit(EXIT_SUCCESS);
  case 3:
	heroArc1 = !heroArc1;
	heroArc2 = false;  //set other views off just in case
	heroArc3 = false;
	/*if(cam.getCurrentMode() != FIRST_PERSON){
      cam.switchMode(FIRST_PERSON);
	  break;
	}
	else if(cam.getCurrentMode() != ARCBALL){	  
      cam.switchMode(ARCBALL);
      break;
	}*/
	break;
  case 4:
    heroArc1 = false;
	heroArc2 = !heroArc2;  //set other views off just in case
	heroArc3 = false;
	break;
	  
  case 5:
    heroArc1 = false;
	heroArc2 = false;  //set other views off just in case
	heroArc3 = !heroArc3;
		break;
		
  case 6:
	if(cam.getCurrentMode() != FREE){
      cam.switchMode(FREE);
	  break;
	}
	else if(cam.getCurrentMode() != ARCBALL){	  
      cam.switchMode(ARCBALL);
      break;
	}
	
	case 9:
	  if(cam.getCurrentMode() != FIRST_PERSON){
        cam.switchMode(FIRST_PERSON);
		cam.mouseRotate(vehicle.theta, 0);
	    break;
	  }
	  else if(cam.getCurrentMode() != ARCBALL){	  
        cam.switchMode(ARCBALL);
        break;
	  }
	case 10:
      if(cam2.getCurrentMode() != FIRST_PERSON){
	    cam2.switchMode(FIRST_PERSON);
		cam2.mouseRotate(vehicle.theta, 0);
	    break;
      }
      else if(cam2.getCurrentMode() != ARCBALL){	  
	    cam2.switchMode(ARCBALL);
	    break;
      }
  }
}

// createMenus() ///////////////////////////////////////////////////////////////
//
//  Handles creating a menu, adding menu entries, and attaching the menu to
//  a mouse button
//
////////////////////////////////////////////////////////////////////////////////
void createMenus() {
	// TODO #01: Create a Simple Menu
  int id = glutCreateMenu(myMenu); //The menu becomes the menu when it is created
  glutAddMenuEntry("Hero1", 3);
  glutAddMenuEntry("Hero2", 4);
  glutAddMenuEntry("Hero3", 5);
  int otherSubId = glutCreateMenu(myMenu); //The menu becomes the menu when it is created
  glutAddMenuEntry("Hero1", 6);
//  glutAddMenuEntry("Hero2", 7);
//  glutAddMenuEntry("Hero3", 8);
  int thirdSubID = glutCreateMenu(myMenu);
  glutAddMenuEntry("Hero1", 9);
  glutAddMenuEntry("Hero2", 10);
//  glutAddMenuEntry("Hero3", 11);
  glutCreateMenu(myMenu);
  glutAddSubMenu("Toggle Viewport", id);
  glutAddSubMenu("Toggle First Person", thirdSubID);
  glutAddSubMenu("Show/Hide Freecam", otherSubId);
  glutAddMenuEntry("Show/Hide CtrlCage", 0);
  glutAddMenuEntry("Show/Hide Curve", 1);
  glutAddMenuEntry("Quit", 2);
  
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// registerCallbacks() /////////////////////////////////////////////////////////
//
//  Register all of our callbacks for GLUT.
//
////////////////////////////////////////////////////////////////////////////////
void registerCallbacks() {
    // keyboard callbacks
    glutSetKeyRepeat(   GLUT_KEY_REPEAT_ON );
    glutKeyboardFunc(   normalKeysDown     );
    glutKeyboardUpFunc(   normalKeysUp     );

    // mouse callbacks
    glutMouseFunc(      mouseCallback      );
    glutMotionFunc(     mouseMotion        );

    // display callbacks
    glutDisplayFunc(    renderScene       );
    glutReshapeFunc(    resizeWindow       );

    // timer callback
    glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

bool loadWorldFile(const char* const filename)
{
  FILE* fp = fopen(filename, "r");
  if(!fp)
  {
    perror("Could not open worldfile");
    return false;
  }

  //load the control curves for heros to follow
  if(!mandrake.loadCtrlPoints(fp))
  {
    fprintf(stderr, "Could not curve load control points from %s.\n", filename);
    return false;
  }
  return true;
}


// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Takes a single command line argument for our 
//    control points file.
//
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
    if(argc < 2)
    {
      printf("Usage: %s ctrl-points.csv\n", argv[0]);
      exit(EXIT_SUCCESS);
    }

    //load world data
    if(!loadWorldFile("controlPoints10.csv"))
    {
      fprintf(stderr, "Could not load worldfile data\n");
      exit(EXIT_SUCCESS);
    }

    // create a double-buffered GLUT window at (50, 50) with predefined window size
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    glutCreateWindow( "DireTiger" );

    fprintf(stdout, "[INFO]: /-------------------------------------------------------------\\\n");
    fprintf(stdout, "[INFO]: | OpenGL Information                                          |\n");
    fprintf(stdout, "[INFO]: |-------------------------------------------------------------|\n");
    fprintf(stdout, "[INFO]: |   OpenGL Version:  %40s |\n", glGetString(GL_VERSION));
    fprintf(stdout, "[INFO]: |   OpenGL Renderer: %40s |\n", glGetString(GL_RENDERER));
    fprintf(stdout, "[INFO]: |   OpenGL Vendor:   %40s |\n", glGetString(GL_VENDOR));
    //fprintf(stdout, "[INFO]: |   GLUI Version:    %40.2f |\n", GLUI_VERSION);
    fprintf(stdout, "[INFO]: \\-------------------------------------------------------------/\n");

    // do some basic OpenGL setup
    initScene();

    // create our menu options and attach to mouse button
    createMenus();

    // register callback functions...
    registerCallbacks();

    //set the camera to watch the ship
    cam.objFollow(&mandrake.position, &mandrake.theta);
    cam2.objFollow(&vehicle.location, &vehicle.theta);
    cam3.objFollow(&wagon.location, &wagon.theta);
    //cam.objWatch(&mandrake.location);

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
