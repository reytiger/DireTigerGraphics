//FPSCounter.h
//Kevin Carbaugh
//Calculates and displays average framerate of openGL
#include <ctime>

class FPSCounter
{
  public:
    FPSCounter();
    void onRender(size_t winWidth, size_t winHeight);

  private:
    void drawFPSCount(size_t winWidth, size_t winHeight);

    time_t lastTime;
    time_t lastUpdate;
    int nSamples;
    double timer;
    double avgFps;

};
