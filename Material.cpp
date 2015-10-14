#include "Material.h"

//ctor
Material::Material()
{
  //default to white
  for(int i = 0; i < 4; ++i)
    diffuseColor[i] = specularColor[i] = ambientColor[i] = 1.f;
}

Material::Material(const GLfloat (&diffCol)[4], const GLfloat (&specCol)[4], const GLfloat (&ambiCol)[4])
{
  for(int i = 0; i < 4; ++i)
  {
    diffuseColor[i] = diffCol[i];
    specularColor[i] = specCol[i];
    ambientColor[i] = ambiCol[i];
  }
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

GLfloat* Material::getDiffuseColor() { return diffuseColor; };
GLfloat* Material::getSpecularColor() { return specularColor; };
GLfloat* Material::getAmbientColor() { return ambientColor; };
