#include "Material.h"

//ctor
Material::Material()
{
  //default to white
  for(int i = 0; i < 4; ++i)
    diffuseColor[i] = specularColor[i] = ambientColor[i] = 1.f;
}

Material::Material(const GLfloat (&diffCol)[4], const GLfloat (&specCol)[4], const GLfloat (&ambiCol)[4], GLfloat shine) : shininess(shine)
{
  for(int i = 0; i < 4; ++i)
  {
    diffuseColor[i] = diffCol[i];
    specularColor[i] = specCol[i];
    ambientColor[i] = ambiCol[i];
  }
}

void Material::glApplyMaterial(const Material& mat)
{
  const GLenum face = GL_FRONT_AND_BACK;
  glMaterialfv(face, GL_DIFFUSE, mat.diffuseColor);
  glMaterialfv(face, GL_SPECULAR, mat.specularColor);
  glMaterialfv(face, GL_AMBIENT, mat.ambientColor);
  glMaterialf(face, GL_SHININESS, mat.shininess * 128.0);
}

void Material::setDiffuseColor(float r, float g, float b, float a)
{
  diffuseColor[0] = r;
  diffuseColor[1] = g;
  diffuseColor[2] = b;
  diffuseColor[3] = a;
}

void Material::setSpecularColor(float r, float g, float b, float a)
{
  specularColor[0] = r;
  specularColor[1] = g;
  specularColor[2] = b;
  specularColor[3] = a;
}

void Material::setAmbientColor(float r, float g, float b, float a)
{
  ambientColor[0] = r;
  ambientColor[1] = g;
  ambientColor[2] = b;
  ambientColor[3] = a;
}

void Material::setShine(GLfloat s)
{
  shininess = s;
}

GLfloat* Material::getDiffuseColor() { return diffuseColor; };
GLfloat* Material::getSpecularColor() { return specularColor; };
GLfloat* Material::getAmbientColor() { return ambientColor; };
GLfloat  Material::getShininess() { return shininess; };
