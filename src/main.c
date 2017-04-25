#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "render.h"

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f, lz=-1.0f, ly = 0.0f;

// XZ position of the camera
float x=0.0f, z=5.0f, y = 1.75f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// width and height of the window
int h,w;

// variables to compute frames per second
int frame;
long time, timebase;

// variables to hold window identifiers
int mainWindow, subWindow1, subWindow2;
//border between subwindows
int border = 6;

void changeSize(int w1,int h1) {
  if(h1 == 0)
    h1 = 1;

  // we're keeping these values cause we'll need them latter
  w = w1;
  h = h1;

  // set subwindow 1 as the active window
  glutSetWindow(subWindow1);
  // resize and reposition the sub window
  glutPositionWindow(border,border);
  glutReshapeWindow(w - 2*border, h - 2*border);
  setProjection(w - 2*border, h - 2*border);

  // set subwindow 2 as the active window
  glutSetWindow(subWindow2);
  // resize and reposition the sub window
  glutPositionWindow(border, h * 2/3);
  glutReshapeWindow(w* 1/3, h * 1/3 - border);
  setProjection(w* 1/3, h * 1/3 - border);
}

// -----------------------------------
//             RENDER CALL
// -----------------------------------

void render2() {
  renderScenesw2(subWindow2, x, y ,z, lx, ly, lz, angle, deltaAngle);
}

void render1() {
  renderScenesw1(subWindow1, x, y, z, lx, ly, lz, frame, time, timebase, w, h);
}

void render() {
  renderSceneAll(mainWindow, subWindow1, subWindow2, &lx, &ly, &lz, &x, &y, &z,
                 deltaMove, deltaAngle, &angle, frame, time, timebase, w, h);
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {

  if (key == 27) {
    glutDestroyWindow(mainWindow);
    exit(0);
  }
}

void pressKey(int key, int xx, int yy) {

  switch (key) {
    case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
    case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
    case GLUT_KEY_UP : deltaMove = 1.0f; break;
    case GLUT_KEY_DOWN : deltaMove = -1.0f; break;
  }
  glutSetWindow(mainWindow);
  glutPostRedisplay();

}

void releaseKey(int key, int x, int y) {

  switch (key) {
    case GLUT_KEY_LEFT :
    case GLUT_KEY_RIGHT : deltaAngle = 0; break;
    case GLUT_KEY_UP :
    case GLUT_KEY_DOWN : deltaMove = 0;break;
  }
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y) {

  // this will only be true when the left button is down
  if (xOrigin >= 0) {

    // update deltaAngle
    deltaAngle = (x - xOrigin) * 0.001f;

    // update camera's direction
    lx = sin(angle + deltaAngle);
    lz = -cos(angle + deltaAngle);

    glutSetWindow(mainWindow);
    glutPostRedisplay();
  }
}

void mouseButton(int button, int state, int x, int y) {

  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {

    // when the button is released
    if (state == GLUT_UP) {
      angle += deltaAngle;
      deltaAngle = 0.0f;
      xOrigin = -1;
    }
    else  {// state = GLUT_DOWN
      xOrigin = x;

    }
  }
}


// action for menu and keyboard
//
void actions(int value) {
  switch (value) {
    case 1:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case 2:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    case 3:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
    case 0:
      exit(0); break;
  }
}


// menu handler function
//
void mainMenu(int value) {
  actions(value);
  glutPostRedisplay();
}

// -----------------------------------
//             MAIN and INIT
// -----------------------------------

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char **argv) {
  createSnowMen();

  // init GLUT and create main window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(800,800);
  mainWindow = glutCreateWindow("Escape the SnowMen");

  // callbacks for main window
  glutDisplayFunc(render);
  glutReshapeFunc(changeSize);

  // sub windows
  subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h-2*border);
  glutDisplayFunc(render1);
  init();

  // register callbacks
  glutIgnoreKeyRepeat(1);
  glutKeyboardFunc(processNormalKeys);
  glutCreateMenu(mainMenu);
  glutAddMenuEntry("Scene mode fils de fer (f)", 1);
  glutAddMenuEntry("Scene mode pleine (g)", 2);
  glutAddMenuEntry("Scene mode points (h)", 3);
  glutAddMenuEntry("Quitter (q)", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
  glutDisplayFunc(render2);

  // enter GLUT event processing cycle
  glutIdleFunc(render);
  glutMainLoop();

  return 1;
}
