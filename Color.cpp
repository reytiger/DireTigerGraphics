#include "Color.h"

Color::Color(float r, float g, float b, float a)
{
  rgba[0] = r;
  rgba[1] = g;
  rgba[2] = b;
  rgba[3] = a;
}

float Color::getRed() { return rgba[0]; };
float Color::getBlue() { return rgba[1]; };
float Color::getGreen() { return rgba[2]; };
float Color::getAlpha() { return rgba[3]; };
