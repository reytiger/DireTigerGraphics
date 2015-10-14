//Color.h - Defines an RGBA color type for use with
//openGl funcfv() type calls

#pragma once
class Color
{
  public:
    Color(float r, float g, float b, float a);
    float getRed();
    float getBlue();
    float getGreen();
    float getAlpha();
    float rgba[4];
};
