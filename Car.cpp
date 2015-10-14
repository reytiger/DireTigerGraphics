#include "Car.h"

Car::Car(double startX, double startY, double startZ) : location(startX, startY, startZ)
{
  wheelAngle = 0;
  updown = 1;
  value = 1;

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

  if(!keysDown[0x73] && !keysDown[0x77])

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


//The main render function
//In charge of all GlStack operations
//as well as managine attributes of each matrix stack frame
void Car::draw()
{
	updown += value;
	glutPostRedisplay();
	if (updown <= 1){
		value = 1;
	} else if (updown >= 2){
		value = -1;
	}
	drawHero();
}
