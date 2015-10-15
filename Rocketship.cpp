#include "Rocketship.h"

Rocketship::Rocketship(float rocketScale) : BezierHero(), rocketScale(rocketScale)
{
  finRotation = 0.f;
  nozzleRotation = 0.f;
  thrustCycle = 0.f;

  //Quadric objects
  bodyCylinder = gluNewQuadric();
  engineNozzle = gluNewQuadric();
  bodyBottomCap = gluNewQuadric();
  
  //Setup draw styles
  gluQuadricDrawStyle(bodyCylinder, GLU_FILL);
  gluQuadricDrawStyle(engineNozzle, GLU_FILL);
  gluQuadricDrawStyle(bodyBottomCap, GLU_FILL);
}

Rocketship::~Rocketship()
{
  gluDeleteQuadric(bodyCylinder);
  gluDeleteQuadric(engineNozzle);
  gluDeleteQuadric(bodyBottomCap);
}

void Rocketship::tick(bool* keysDown)
{
  finRotation += 0.3f;
  nozzleRotation -= 0.3f;
  thrustCycle += 0.2f;

  //Movement of the character based on keyboard
  //w
  if(keysDown[0x77])
  {
    setThrustState(true);
    move(true);
  }
  //s
  if(keysDown[0x73])
  {
    setThrustState(true);
    move(false);
  }

  if(!keysDown[0x73] && !keysDown[0x77])
    setThrustState(false);

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

void Rocketship::move(bool forward)
{
  //determine our heading vector
  float dX, dZ;
  dX = sin(theta * DEG2RAD);
  dZ = cos(theta * DEG2RAD);

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
  position += Point<GLfloat>(moveSpeed * dX, 0.f, 0.f);
  position += Point<GLfloat>(0.f, 0.f, moveSpeed * dZ);
  //check to see if our movement would put us out of bounds
  
  //Maybe TODO: make this depend on the variable in main
  int gridSize = 200 / 2;
  //x bounds
  if(position.getX() > gridSize)
    position.setX(gridSize);
  if(position.getX() < -gridSize)
    position.setX(-gridSize);
  //and now z
  if(position.getZ() > gridSize)
    position.setZ(gridSize);
  if(position.getZ() < -gridSize)
    position.setZ(-gridSize);

}

void Rocketship::setThrustState(bool yn)
{
  thrustState = yn;
}

//The main render function
//In charge of all GlStack operations
//as well as managine attributes of each matrix stack frame
void Rocketship::draw()
{
  //start with the body
  glPushMatrix();

    //set the heading, rotated on the xz plane (normal to y = 1)
    glRotatef(theta, 0, 1, 0);

    //scale by whatever factor was set by the ctor
    glScalef(rocketScale, rocketScale, rocketScale);

    glColor3ub(170, 181, 173);
    drawBody();

  drawName();
    //Draw the rocket fins
    glPushMatrix();
    {
      glColor3ub(211, 132, 86);
      //rotate from the z=0 plane to be placed along the body
      glRotatef(90, 0, 1, 0);

      //draw our fins around the outsize of the cylinder
      for(int i = 0; i < 4; ++i)
      {
        glPushMatrix();
          //the x vector is now along the body's axis
          glRotatef(i * 90 + finRotation, 1, 0, 0);
          glTranslatef(-8, 3, 0);
          glScalef(0.8, 0.8, 1);
          drawFin();
        glPopMatrix();
      }
    }
    glPopMatrix();

    //Draw the engine nozzles on the underside
    glPushMatrix();
    {
      glColor3ub(175, 209, 203);
      //emplace on the bottom of the rocket
      glRotatef(180, 1, 0, 0);
      for(int i = 0; i < 3; ++i)
      {
        glPushMatrix();
          //the z vector is along the body
          glRotatef(i * 120 + nozzleRotation, 0, 0, 1);
          glTranslatef(1.3, 0, 0);
          drawNozzle();
        glPopMatrix();
      }
    }
    glPopMatrix();

    //Draw the nosecone
    glPushMatrix();
    {
      //use the same color as the fins
      glColor3ub(211, 132, 86);
      glTranslatef(0, 0, 25);
      drawNose();
    }
    glPopMatrix();

    //draw the thrust, but coniditionally
    if(thrustState)
    {
      glPushMatrix();
      {
        //A nice rich golden orange
        glColor3ub(244, 183, 26);
        glRotatef(180, 1, 0, 0);

        //Place over the nozzles
        glTranslatef(0, 0, 1);
        for(int i = 0; i < 3; ++i)
        {
          glPushMatrix();
            glRotatef(i * 120 + nozzleRotation, 0, 0, 1);
            glTranslatef(1.3, 0, 0);
            float thrustScaler = sin(thrustCycle);
            if(thrustScaler < 0)
              thrustScaler = -thrustScaler;
            glScalef(1, 1, 1.4 * thrustScaler + 0.2);
            drawThrust();
          glPopMatrix();
        }
      }
      glPopMatrix();
    }
  glPopMatrix();
}

void Rocketship::drawBody()
{
  gluCylinder(bodyCylinder, 3., 3., 25., 40., 4.);
  //fills in the bottom. The top is covered by the nosecone
  gluDisk(bodyBottomCap, 0, 3, 40, 20);
}


void Rocketship::render()
{
  glPushMatrix();
    BezierHero::render();
    draw();
  glPopMatrix();
}


void Rocketship::drawFin()
{
  glBegin(GL_TRIANGLES);
  //a nice, right triangle
  glVertex3f(0, 0, 0);
  glVertex3f(8, 0, 0);
  glVertex3f(8, 6, 0);
  glEnd();
}

void Rocketship::drawNozzle()
{
  gluCylinder(engineNozzle, 1, 1, 0.5, 30, 1);
}

void Rocketship::drawNose()
{
  glutSolidCone(3.,10., 30, 10);
}

void Rocketship::drawThrust()
{
  glutSolidCone(1, 3, 20, 3);
}

void Rocketship::drawName()
{
	char scrtext[64]= "<Kevin>";

	// choose a colour
	glPushMatrix();
	glColor3ub(0, 0, 255);
	// where we want it written
	glTranslatef(3, 10, 8);
	glRotatef(180, 0, 1, 0);
	// how big we want it
	glScalef(.01, .01, .01);
	for (int c=0; scrtext[c] != 0; ++c)
	glutStrokeCharacter(GLUT_STROKE_ROMAN, scrtext[c]);
	glPopMatrix(); 
}
