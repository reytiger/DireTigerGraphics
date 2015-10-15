// Ryan Porter -- character vehicle
#include "Wagon.h"

// Constructors
Wagon::Wagon()  {
	characterX = 0;
	characterY = 0;
	characterZ = 0;
	
	theta = 0;
	wheelTheta = 0;
}

// Constructors
Wagon::Wagon(float x, float y, float z, float th) {
	characterX = x;
	characterY = y;
	characterZ = z;
	
	theta = th;
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

void Wagon::drawName(){
	char scrtext[64]= "<Ryan>";

	// choose a colour
	glPushMatrix();
	glColor3ub(0, 0, 255);
	// where we want it written
	glTranslatef(2, 5, 0);
	glRotatef(180, 0, 1, 0);
	// how big we want it
	glScalef(.01, .01, .01);
	for (int c=0; scrtext[c] != 0; ++c)
	glutStrokeCharacter(GLUT_STROKE_ROMAN, scrtext[c]);
	glPopMatrix(); 
}

void Wagon::draw() {
	/* Function that calls the  functions for drawing my 'character,' 
	 * which will be a wizard's wagon.
	 */
	 glPushMatrix();
	 
	 // Function for drawing the body of the wagon
	 
	 {
		glRotatef(0.0, theta, 0.0, 1.0);
		drawBox();
		glRotatef(0.0, -theta, 0.0, 1.0);
	 }
	 drawName();
	 glPopMatrix();
	 
	 // This function is easily adapted to a more complex character construct.
}

void Wagon::timetick(bool* keysDown) {
	// Function for updating character
	if (abs(characterX + movementConstant*(sinf(theta))) < 45 && abs(characterZ + movementConstant*(cosf(theta))) < 45) {
			characterX += movementConstant*(sinf(theta));
			characterZ += movementConstant*(cosf(theta));
	}
    if (keysDown[0x71] || keysDown[0x51]) {
        exit(0);
	}
	else if (keysDown[0x77] || keysDown[0x57]) {
		if (abs(characterX + movementConstant*(sinf(theta))) < 45 && abs(characterZ + movementConstant*(cosf(theta))) < 45) {
			characterX += movementConstant*(sinf(theta));
			characterZ += movementConstant*(cosf(theta));
			wheelTheta += 10;
		}
	}
	else if (keysDown[0x73] || keysDown[0x53]) {
		if (abs(characterX - movementConstant*(sinf(theta))) < 45 && abs(characterZ - movementConstant*(cosf(theta))) < 45) {
			characterX -= movementConstant*(sinf(theta));
			characterZ -= movementConstant*(cosf(theta));
			wheelTheta -= 10;
		}
	}
	else if (keysDown[0x61] || keysDown[0x41]) {
		theta += M_PI / 20.0f;
	}
	else if (keysDown[0x64]) {
		theta -= M_PI / 20.0f;
	}
	// Needs to be set up yet
}
/*void Wagon::updateCharacterForward() {
	// Function for moving the wagon forward
	if (abs(characterX + movementConstant*(sinf(characterTheta))) < 45 && abs(characterZ + movementConstant*(cosf(characterTheta))) < 45) {
			characterX += movementConstant*(sinf(characterTheta));
			characterZ += movementConstant*(cosf(characterTheta));
	}
    if (keysDown[0x71] || keysDown[0x51] || key == 27) {
        exit(0);
	}
	else if (keysDown[0x77] || keysDown[0x57]) {
		if (abs(characterX + movementConstant*(sinf(theta))) < 45 && abs(characterZ + movementConstant*(cosf(theta))) < 45) {
			characterX += movementConstant*(sinf(theta));
			characterZ += movementConstant*(cosf(theta));
			wheelTheta += 10;
		}
	}
	else if (keysDown[0x73] || keysDown[0x53]) {
		if (abs(characterX - movementConstant*(sinf(theta))) < 45 && abs(characterZ - movementConstant*(cosf(theta))) < 45) {
			characterX -= movementConstant*(sinf(theta));
			characterZ -= movementConstant*(cosf(theta));
			wheelTheta -= 10;
		}
	}
	else if (keysDown[0x61] || keysDown[0x41]) {
		theta += M_PI / 20.0f;
	}
	else if (keysDown[0x64] || key[0x44]) {
		theta -= M_PI / 20.0f;
	}
}*/

void Wagon::updateCharacterLeft() {
	// Function for turning left
	theta += M_PI / 20.0f;
}

void Wagon::updateCharacterRight() {
	// Function for turning right
	theta -= M_PI / 20.0f;
}
