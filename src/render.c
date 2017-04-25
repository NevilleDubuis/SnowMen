#include <math.h>
#include "render.h"

// variables to compute and display frames per second
int frame;
long time, timebase;
char displayFps[50];

void createSnowMen() {
  int index = 0;

  puts("Start SnowMen Init");

  for(int i = 0; i < 5; i++) {
    for(int j= 0; j < 5; j++) {
      snowMen[index].x = i*10.0f;
      snowMen[index].y = 0.0f;
      snowMen[index].z = j * 10.0f;
      index++;
    }
  }

  puts("End SnowMen Init");
}

void renderSnowMan(SnowMan *snowMan) {
  glPushMatrix();
  glTranslatef(snowMan->x, snowMan->y, snowMan->z);

  glColor3f(1.0f, 1.0f, 1.0f);

  // Draw Body
  glTranslatef(0.0f ,0.75f, 0.0f);
  glutSolidSphere(0.75f,20,20);

  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f,20,20);

  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f,0.0f,0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f,10,10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f,10,10);
  glPopMatrix();

  // Draw Nose
  glColor3f(1.0f, 0.5f, 0.5f);
  glRotatef(0.0f,1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f,0.5f,10,2);

  glColor3f(1.0f, 1.0f, 1.0f);

  glPopMatrix();
}


void setProjection(int w1, int h1) {
  float ratio;
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  ratio = 1.0f * w1 / h1;
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w1, h1);

  // Set the clipping volume
  gluPerspective(45,ratio,0.1,1000);
  glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(
    float x,
    float y,
    float z,
    void *font,
    char *string) {

  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void restorePerspectiveProjection() {

  glMatrixMode(GL_PROJECTION);
  // restore previous projection matrix
  glPopMatrix();

  // get back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection(int width, int height) {

  // switch to projection mode
  glMatrixMode(GL_PROJECTION);

  // save previous matrix which contains the
  //settings for the perspective projection
  glPushMatrix();

  // reset matrix
  glLoadIdentity();

  // set a 2D orthographic projection
  gluOrtho2D(0, width, height, 0);

  // switch back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}

void computePos(float *x, float *z, float lx, float lz, float deltaMove) {
  *x += deltaMove * lx * 0.1f;
  *z += deltaMove * lz * 0.1f;
}

// add light to Scene
//
void renderLights() {
  static float amb[] =  {0.4, 0.4, 0.4, 0.5};
  static float dif[] =  {1.0, 1.0, 1.0, 0.0};

  float light_diffuse[] = {1.0, 1.0, 1.0, 0.5};
  float light_position[] = {-1.0, 1.0, 1.0, 0.0};

  // set lights
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
}

void moveSnowMan(SnowMan *snowMan, float playerX, float playerZ, int index) {
  const float minDistance = 1.05f;
  float distance, newX, newZ;
  int applyNew = 1, applyNewX = 0, applyNewZ = 0;

  if (playerX > snowMan->x) {
    newX = snowMan->x + 0.01f;
  } else {
    newX = snowMan->x - 0.01f;
  }

  if (playerZ > snowMan->z) {
    newZ = snowMan->z + 0.01f;
  } else {
    newZ = snowMan->z - 0.01f;
  }

  for(int i = 0; i < 25; i++) {
    if (i != index) {
      distance = sqrt(pow((newX - snowMen[i].x), 2) + pow((newZ - snowMen[i].z), 2));

      if (abs(distance) < minDistance) {
        applyNew = 0;

        distance = sqrt(pow((newX - snowMen[i].x), 2) + pow((snowMan->z - snowMen[i].z), 2));
        if (abs(distance) > minDistance) {
          applyNewX = 1;
        } else {
          applyNewX = 0;

          distance = sqrt(pow((snowMan->x - snowMen[i].x), 2) + pow((newZ - snowMen[i].z), 2));
          if (abs(distance) > minDistance) {
            applyNewZ = 1;
          } else {
            applyNewZ = 0;
          }
        }
      }
    }
  }

  if (applyNew == 1) {
    snowMan->x = newX;
    snowMan->z = newZ;
  } else {
    if (applyNewX == 1) {
      snowMan->x = newX;
    }
    if (applyNewZ == 1) {
      snowMan->z = newZ;
    }
  }
}

// Common Render Items for all subwindows
void renderScene2(float x, float z) {
  // Draw ground
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f, -100.0f);
  glEnd();

  for(int i = 0; i < 25; i++) {
    moveSnowMan(&snowMen[i], x, z, i);
    renderSnowMan(&snowMen[i]);
  }
}

// Display func for main window
void renderScene(int mainWindow) {
  glutSetWindow(mainWindow);
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
}

// Display func for sub window 1
void renderScenesw1(int subWindow1,
                    float x, float y, float z,
                    float lx, float ly, float lz,
                    int width, int height) {

  glutSetWindow(subWindow1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(x, y, z,
      x + lx, y + ly, z + lz,
      0.0f, 1.0f, 0.0f);

  renderScene2(x, z);

  // display fps in the top window
  frame++;

  time = glutGet(GLUT_ELAPSED_TIME);
  if (time - timebase > 1000) {
    sprintf(displayFps, "FPS:%4.2f",
      frame * 1000.0 / (time - timebase));
    timebase = time;
    frame = 0;
  }

  setOrthographicProjection(width, height);

  glPushMatrix();
  glLoadIdentity();
  renderBitmapString(5, 30, 0, GLUT_BITMAP_HELVETICA_12, displayFps);
  glPopMatrix();

  restorePerspectiveProjection();

  glutSwapBuffers();
}

// Display func for sub window 2
void renderScenesw2(int subWindow,
                    float x, float y, float z,
                    float lx, float ly, float lz,
                    float angle, float deltaAngle) {

  glutSetWindow(subWindow);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(x, y+50, z,
      x , y - 1, z,
      lx, 0, lz);

  // Draw red cone at the location of the main camera
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(x, y, z);
  glRotatef(180-(angle+deltaAngle)*180.0 / 3.14, 0.0, 1.0, 0.0);
  glutSolidCone(0.2, 0.8f, 4, 4);
  glPopMatrix();

  renderScene2(x, z);

  glutSwapBuffers();
}

// Global render func
void renderSceneAll(int mainWindow, int subWindow1, int subWindow2,
                    float *lx, float *ly, float *lz, float *x, float *y, float *z,
                    float deltaMove, float deltaAngle, float *angle,
                    int width, int height) {

  // check for keyboard movement
  if (deltaMove) {
    computePos(x, z, *lx, *lz, deltaMove);
  }

  if (deltaAngle) {
    *lx = sin(*angle + deltaAngle);
    *lz = -cos(*angle + deltaAngle);
    *angle = *angle + deltaAngle;
  }

  if (deltaMove || deltaAngle) {
    glutSetWindow(mainWindow);
    glutPostRedisplay();
  }

  renderLights();
  renderScene(mainWindow);
  renderScenesw1(subWindow1, *x, *y, *z, *lx, *ly, *lz, width, height);
  renderScenesw2(subWindow2, *x, *y, *z, *lx, *ly, *lz, *angle, deltaAngle);
}
