#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

typedef struct SnowMan SnowMan;
struct SnowMan
{
  float x;
  float y;
  float z;
};

SnowMan snowMen[25];

void createSnowMen();

void renderSnowMan(SnowMan *snowMan);

void setProjection(int w1, int h1);

void renderBitmapString(float x, float y, float z,
                        void *font, char *string);

void restorePerspectiveProjection();

void setOrthographicProjection();

void computePos(float *x, float *z, float lx, float lz, float deltaMove);

void renderLights();

void renderScene2();

void renderScene(int mainWindow);

void renderScenesw1(int subWindow,
                    float x, float y, float z,
                    float lx, float ly, float lz,
                    int frame, long time, long timebase,
                    int width, int height);

void renderScenesw2(int subWindow,
                    float x, float y, float z,
                    float lx, float ly, float lz,
                    float angle, float deltaAngle);

void renderSceneAll(int mainWindow, int subWindow1, int subWindow2,
                    float *lx, float *ly, float *lz, float *x, float *y, float *z,
                    float deltaMove, float deltaAngle, float *angle,
                    int frame, long time, long timebase, int width, int height);
