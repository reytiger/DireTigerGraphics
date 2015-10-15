//Material.h - Specifies a material for an openGL object

#pragma once

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

class Material
{
  public:
    Material();
    Material(const GLfloat (&diffCol)[4], const GLfloat (&specCol)[4], const GLfloat (&ambiCol)[4], GLfloat shine);

    static void glApplyMaterial(const Material& mat);

    void setDiffuseColor(float r, float g, float b, float a);
    void setSpecularColor(float r, float g, float b, float a);
    void setAmbientColor(float r, float g, float b, float a);
    void setShine(GLfloat s);

    GLfloat* getDiffuseColor();
    GLfloat* getSpecularColor();
    GLfloat* getAmbientColor();
    GLfloat getShininess();

  private:
    GLfloat diffuseColor[4];
    GLfloat specularColor[4];
    GLfloat ambientColor[4];
    GLfloat shininess;
};
