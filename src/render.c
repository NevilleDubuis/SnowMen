#include "render.h"

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


void drawSnowMan() {
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
  static float amb[] =  {0.4, 0.4, 0.4, 0.0};
  static float dif[] =  {1.0, 1.0, 1.0, 0.0};

  float light_diffuse[] = {1.0, 0.0, 1.0, 1.0};
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

// Common Render Items for all subwindows
void renderScene2() {
  // Draw ground
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f,  100.0f);
    glVertex3f( 100.0f, 0.0f, -100.0f);
  glEnd();

  // Draw 36 SnowMen
  for(int i = -5; i < 5; i++)
    for(int j=-5; j < 5; j++)
    {
      glPushMatrix();
      glTranslatef(i*10.0f, 0.0f, j * 10.0f);
      drawSnowMan();
      glPopMatrix();
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
                    int frame, long time, long timebase,
                    int width, int height) {
  char s[50];

  glutSetWindow(subWindow1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(x, y, z,
      x + lx, y + ly, z + lz,
      0.0f, 1.0f, 0.0f);

  renderScene2();

  // display fps in the top window
  frame++;

  time=glutGet(GLUT_ELAPSED_TIME);
  if (time - timebase > 1000) {
    sprintf(s, "FPS:%4.2f",
      frame * 1000.0 / (time-timebase));
    timebase = time;
    frame = 0;
  }

  setOrthographicProjection(width, height);

  glPushMatrix();
  glLoadIdentity();
  renderBitmapString(5, 30, 0, GLUT_BITMAP_HELVETICA_12, s);
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

  renderScene2();

  glutSwapBuffers();
}

// Global render func
void renderSceneAll(int mainWindow, int subWindow1, int subWindow2,
                    float *lx, float *ly, float *lz, float *x, float *y, float *z,
                    float deltaMove, float deltaAngle, float *angle,
                    int frame, long time, long timebase, int width, int height) {

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
  renderScenesw1(subWindow1, *x, *y, *z, *lx, *ly, *lz, frame, time, timebase, width, height);
  renderScenesw2(subWindow2, *x, *y, *z, *lx, *ly, *lz, *angle, deltaAngle);
}
