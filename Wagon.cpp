// Ryan Porter -- character vehicle
#include "Wagon.h"

// Constructors
Wagon::Wagon() {
	characterX = 0;
	characterY = 0;
	characterZ = 0;
	
	characterTheta = 0;
	wheelTheta = 0;
}

// Constructors
Wagon::Wagon(float x, float y, float z, float cth) {
	characterX = x;
	characterY = y;
	characterZ = z;
	
	characterTheta = cth;
	wheelTheta = 0;
}

// Functions for drawing my character ----------------------------------------------
void Wagon::drawBench() {
	// Function for drawing the bench of the wagon
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	
	glScalef(3.2, 1.0, 1);
	glutSolidCube(1);
	glScalef(0.3125, 1.0, 1.0);
	
	glPopMatrix();
}

void Wagon::drawWheel() {
	// This function simply draws a single wheel
	glPushMatrix();
	
	glColor3f(0.1, 0.1, 0.1);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-wheelTheta, 0.0, 0.0, 1.0);
	glutSolidTorus(2.5, 2, 8, 8);
	glRotatef(wheelTheta, 0.0, 0.0, 1.0);
	glScalef(2.0, 2.0, 2.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	
	glPopMatrix();	
}

void Wagon::drawWheels() {
	// Function for drawing the wheels of the wagon
	glPushMatrix();
	
	// Draw Front Left Wheel
	glTranslatef(3.0, 0.0, 2.1);
	drawWheel();
	
	// Draw Front Right Wheel
	glTranslatef(-6.0, 0.0, 0.0);
	drawWheel();
	
	// Draw Rear Right Wheel
	glTranslatef(0.0, 0.0, -4.2);
	drawWheel();
	
	// Draw Rear Left Wheel
	glTranslatef(6.0, 0.0, 0.0);
	drawWheel();
	glTranslatef(0.0, 0.0, 2.1);
	
	glPopMatrix();
}

void Wagon::drawBox() {
	// Function for drawing the main box of the wagon
	glPushMatrix();
	
	// Draw the box
	glColor3f(0.7, 0.0, 0.0);
	glTranslatef(0.0, 3.0, 0.0);
	glScalef(2.0, 1.0, 3.2);
	glutSolidCube(2);
	glScalef(0.50, 1.0, 0.3125);
	
	// Call the function for drawing the bench
	glTranslatef(0.0, 1.0, 1.0);
	drawBench();
	glTranslatef(0.0, -1.0, -1.0);
	
	// Call the function for drawing the wheels
	glTranslatef(0.0, -1.0, 0.0);
	drawWheels();
	glTranslatef(0.0, -2.0, 0.0);
	
	glPopMatrix();
}

void Wagon::drawWagon() {
	/* Function that calls the  functions for drawing my 'character,' 
	 * which will be a wizard's wagon.
	 */
	 glPushMatrix();
	 
	 // Function for drawing the body of the wagon
	 drawBox();
	 
	 glPopMatrix();
	 
	 // This function is easily adapted to a more complex character construct.
}

void updateCharacterForward() {
	// Function for moving the wagon forward
	if (abs(characterX + movementConstant*(sinf(characterTheta))) < 45 && abs(characterZ + movementConstant*(cosf(characterTheta))) < 45) {
			characterX += movementConstant*(sinf(characterTheta));
			characterZ += movementConstant*(cosf(characterTheta));
			wheelTheta += 10;
	}
}

void Wagon::updateCharacterBackward() {
	// Function for moving the wagon backward
	if (abs(characterX - movementConstant*(sinf(characterTheta))) < 45 && abs(characterZ - movementConstant*(cosf(characterTheta))) < 45) {
			characterX -= movementConstant*(sinf(characterTheta));
			characterZ -= movementConstant*(cosf(characterTheta));
			wheelTheta -= 10;
	}
}

void Wagon::updateCharacterLeft() {
	// Function for turning left
	characterTheta += M_PI / 20.0f;
}

void Wagon::updateCharacterRight() {
	// Function for turning right
	characterTheta -= M_PI / 20.0f;
}