#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>

#define GEAR 1
#define PI 3.14159265


GLfloat rotx=0.0;
GLfloat roty=0.0;

GLfloat global_ambient_on[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat global_ambient_off[] = {0.5f, 0.5f, 0.5f, 0.0f};

GLfloat dir_ambient[] = {0.3, 0.3, 0.3, 1.0};
GLfloat dir_diffuse[] = {0.4, 0.4, 0.4, 1.0};
GLfloat dir_specular[] = {1.0, 1.0, 1.0, 1.0};

GLfloat dir_position[] = {1.0,1.0, 1.0, 0.0};

GLfloat mat_specular[] = {0.6,0.6,0.6,1.0};

GLint n_teeth = 8;

GLfloat t_radius;
GLfloat g_rot;
GLfloat g_speed = 1;

bool inv_rot = false;
bool light = false;

float toRad(float angle) {
  return ((angle)*PI)/180;
}

void init (void)
{
 
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_on);

   glLightfv (GL_LIGHT0, GL_AMBIENT, dir_ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, dir_diffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, dir_specular);
   glLightfv (GL_LIGHT0, GL_POSITION, dir_position);
   glEnable(GL_LIGHT0);

   
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   glMaterialfv(GL_FRONT, GL_SPECULAR,mat_specular);
   glMateriali(GL_FRONT, GL_SHININESS,30);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);   

   glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);

   glNewList(GEAR,GL_COMPILE);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0,0,-10);
      for(float angle=0; angle <=360; angle +=0.5) 
        glVertex3f(60*cos(toRad(angle)),60*sin(toRad(angle)),-10);
      glEnd();

      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0,0,10);
      for(float angle=0; angle <=360; angle +=0.5) 
        glVertex3f(60*cos(toRad(angle)),60*sin(toRad(angle)),10);
      glEnd();

      glBegin(GL_QUAD_STRIP);
      for(float angle=0; angle <=360; angle +=0.5) {
        glVertex3f(60*cos(toRad(angle)),60*sin(toRad(angle)),10);
        glVertex3f(60*cos(toRad(angle)),60*sin(toRad(angle)),-10);
      }
      glEnd();

      for(float i=0; i<n_teeth;i++) {
        t_radius = (360/n_teeth)*i;
        glPushMatrix();
          glRotatef(t_radius,0,0,1);
          glTranslatef(0,((60*PI)/16)+50,0);
          glScalef((60*PI*2)/16,20,20);
          glutSolidCube(1);
        glPopMatrix();
      }
   glEndList();
}

void drawScene() {
  glInitNames();
  glPushName(0);


    glPushMatrix();
      glColor3f(0.5,0.5,0.5);
      glRotatef(180,1,0,0);
      glRotatef(-g_rot,0,0,1);
      glLoadName(GEAR);
      glCallList(GEAR);
    glPopMatrix();

    glPushMatrix();
      glColor3f(0.3,0.3,0.3);
      glTranslatef(132,0,0);
      glRotatef(180,1,0,0);
      glRotatef(g_rot,0,0,1);
      glRotatef(22.5,0,0,1);
      glLoadName(GEAR);
      glCallList(GEAR);
    glPopMatrix();
}

void display (void) {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
    glTranslatef(0,40,-350);
    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);
    glRotatef(-45,0,0,1);
      drawScene();
  glPopMatrix();

  glutSwapBuffers();
}

void processSelection(int xPos, int yPos)
  {
  GLfloat fAspect;  // Screen aspect ratio

  // Space for selection buffer
  GLuint selectBuff[64];

  // Hit counter and viewport storeage
  GLint hits, viewport[4];

  // Setup selection buffer
  glSelectBuffer(64, selectBuff);
  
  // Get the viewport
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Switch to projection and save the matrix
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  // Change render mode
  glRenderMode(GL_SELECT);


  glLoadIdentity();
  gluPickMatrix(xPos, viewport[3] - yPos, 4,4, viewport);

  
  fAspect = (float)viewport[2] / (float)viewport[3];
  gluPerspective(45.0f, fAspect, 1.0, 800.0);
      
      display();

  hits = glRenderMode(GL_RENDER);

  if(hits >= 1) {
    //ProcessSelection(selectBuff);
    inv_rot = !inv_rot;
  }
  else
      std::cout << "Hai cliccato spazio vuoto" << std::endl;

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
  }

void reshape(int w, int h) {
   // Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	float fAspect = (GLfloat)w/(GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 800.0);

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void mouse_callback(int button, int state, int x,int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      processSelection(x,y);
}

void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 'w':
         rotx+=5.0;
         break;
      case 's':
         rotx-=5.0;
         break;
	    case 'a':
         roty+=5.0;
         break;
      case 'd':
         roty-=5.0;
         break;
      case 'l':
         light = !light;   
         if(light) {
            glEnable(GL_LIGHT0);
            global_ambient_on[4] = 1.0f;       
         }
         else {
            glDisable(GL_LIGHT0);
            global_ambient_on[4] = 0.0f;    
          }
         break;
      case '+':
         g_speed += 0.1;
         break;
      case '-':
         g_speed -= 0.1;
         break;
   }
   glutPostRedisplay();
}

void idle() {
  if(!inv_rot)
    g_rot -= g_speed;
  else
    g_rot += g_speed;
  glutPostRedisplay();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1200, 800);
   glutCreateWindow ("Gears");
   init ();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutMouseFunc(mouse_callback);
   glutIdleFunc(idle);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0; 
}
