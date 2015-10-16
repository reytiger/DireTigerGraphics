#include "Car.h"
#include <vector>

Car::Car(double startX, double startY, double startZ) : location(startX, startY, startZ)
{
  wheelAngle = 0;
  updown = 1;
  value = 1.0f;

	// Headlights setup and placement
	Color diffCol(1.0f, 1.0f, 1.0f, 1.0f);
  Color ambientCol(0.1f, 0.1f, 0.1f, 1.0f);
  Color specularCol(0.2f, 0.2f, 0.2f, 1.f);
  Point<float> pos1(-2.0f, 0.0f, 5.0f);
	Point<float> pos2(2.0f, 0.0f, 5.0f);
	Vector<float> dir = Vector<float>(0.0f, 0.0f, 1.0f);
	
	leftHeadlight = new Light(SPOT, pos1, diffCol, specularCol, ambientCol, dir);
	rightHeadlight = new Light(SPOT, pos2, diffCol, specularCol, ambientCol, dir);
}


Car::~Car()
{
  delete leftHeadlight;
  delete rightHeadlight;
}


void Car::drawWheels(){
	glPushMatrix();{
		glTranslatef(-3.5, 1, 0);
		glColor3ub(10, 10, 10);
		glRotatef(90, 0, 1, 0);
		glRotatef(wheelAngle, 0, 0, 1);
		gluCylinder(gluNewQuadric(), 1, 1, 2, 15, 15);
		glTranslatef(0, 0, 2);
		gluDisk(gluNewQuadric(), 0, 1, 15, 15);
		glTranslatef(0, 0, 2);
		gluCylinder(gluNewQuadric(), 1, 1, 2, 15, 15);
		gluDisk(gluNewQuadric(), 0, 1, 15, 15);
	}
	glPopMatrix();	
	
}

//the hero function, which calls the wheels function as well.
void Car::drawHero(){
	
	for (int i = -3; i < 3; i++){
		for (int j = -5; j < 5; j++){
			glPushMatrix();
			glTranslatef(0, 2, 0);
			glColor3ub(152, 60, 175);
			glTranslatef(i, 0, j);
			if (i == 0 && j == 2){
				glColor3ub(0, 60, 175);
				glRotatef(270, 1, 0, 0);
				glScalef(1, 1, updown);
				glutSolidCone(1, 2, 10, 10);
			}
			glutSolidCube(1);
			glPopMatrix();
		}
	}
	
	glPushMatrix();
	glTranslatef(0, 0, 2);
	drawWheels();
	glTranslatef(0, 0, -6);
	drawWheels();
	glPopMatrix();
	
}

void Car::tick(bool* keysDown)
{
  

  //Movement of the character based on keyboard
  //w
  if(keysDown[0x77])
  {
	wheelAngle += 15;
    move(true);
  }
  //s
  if(keysDown[0x73])
  {
    wheelAngle -= 15;
    move(false);
  }

  //Rotation
  //d
  if(keysDown[0x64])
  {
    theta -= rotateSpeed;
  }
  //a
  if(keysDown[0x61])
  {
    theta += rotateSpeed;
  }
}

void Car::move(bool forward)
{
  //determine our heading vector
  float dX, dZ;
  dX = sin(theta * DEGREE2RAD);
  dZ = cos(theta * DEGREE2RAD);

  //normalize vector
  float mag = sqrt(dX * dX + dZ * dZ);
  dX /= mag;
  dZ /= mag;
  
  if(!forward)
  {
    //invert the vector
    dX = -dX;
    dZ = -dZ;
  }

  //move our rocket
  location += Point<GLfloat>(moveSpeed * dX, 0.f, 0.f);
  location += Point<GLfloat>(0.f, 0.f, moveSpeed * dZ);
  //check to see if our movement would put us out of bounds
  
  //Maybe TODO: make this depend on the variable in main
  int gridSize = 200 / 2;
  //x bounds
  if(location.getX() > gridSize)
    location.setX(gridSize);
  if(location.getX() < -gridSize)
    location.setX(-gridSize);
  //and now z
  if(location.getZ() > gridSize)
    location.setZ(gridSize);
  if(location.getZ() < -gridSize)
    location.setZ(-gridSize);

}

void Car::drawName(){
	char scrtext[64]= "<Maeva Frostfall>";

	// choose a colour
	glPushMatrix();
	glColor3ub(0, 0, 255);
	// where we want it written
	glTranslatef(5, 5, 0);
	glRotatef(180, 0, 1, 0);
	// how big we want it
	glScalef(.01, .01, .01);
	for (int c=0; scrtext[c] != 0; ++c)
	glutStrokeCharacter(GLUT_STROKE_ROMAN, scrtext[c]);
	glPopMatrix(); 
}

//The main render function
//In charge of all GlStack operations
//as well as managine attributes of each matrix stack frame
void Car::draw()
{
	glPushMatrix();
	glTranslatePoint(location);
	glRotatef(theta, 0, 1, 0);
	updown += value;
	glutPostRedisplay();
	if (updown <= 1.0f){
		value = 0.01;
	} else if (updown >= 2.0f){
		value = -0.01;
	}
	drawHero();
	drawName();
	glPopMatrix();
}
