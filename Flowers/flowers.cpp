#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>

#define HILL 1
#define STEM 2
#define PETAL 3

GLfloat  global_ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f};

GLfloat pos_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat pos_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat pos_specular[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat pos_position[] = { 50.0, 80.0, -5.0, 1.0 };

GLfloat mat_specular[] =  { 0.6f, 0.6f, 0.6f, 1.0f };

GLfloat roty = 0;
GLfloat rotx = 0;

// Parabola concavity
GLfloat alpha = -0.002;

// Min number of petals
GLint n_petals = 12;

// Radius stem
GLfloat s_radius = 1;

// Radius petals
GLfloat p_radius;
GLfloat rad_dist = 9;

// Petal falling
GLfloat p_trasly = 0;
GLfloat p_rotx = 0;

// Animation trigger
bool anim = false;

// Nice func :)
GLfloat toRad(GLfloat deg) {
	return (deg*M_PI)/180;
}

void init () {
   
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

   glLightfv (GL_LIGHT0, GL_AMBIENT, pos_ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, pos_diffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, pos_specular);
   glLightfv (GL_LIGHT0, GL_POSITION, pos_position);
   glEnable(GL_LIGHT0);
   
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   glMaterialfv(GL_FRONT, GL_SPECULAR,mat_specular);
   glMateriali(GL_FRONT, GL_SHININESS,30);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);

   glEnable(GL_DEPTH_TEST);   
   glEnable(GL_NORMALIZE);

   glShadeModel(GL_SMOOTH);

   // Hill
   glNewList(HILL,GL_COMPILE);
   	glColor3f(0,1,0);
   	
   	for(float x = -50; x < 50; x += 0.5) {
   		for(float z = -50; z < 50; z += 0.5) {
   			glBegin(GL_QUADS);
	   			glVertex3f(x,alpha*(pow(x,2)+pow(z,2)),z);
				glVertex3f(x,alpha*(pow(x,2)+pow(z+0.5,2)),z+0.5);
				glVertex3f(x+0.5,alpha*(pow(x+0.5,2)+pow(z+0.5,2)),z+0.5);
				glVertex3f(x+0.5,alpha*(pow(x+0.5,2)+pow(z,2)),z);
   			glEnd();
   		}
   	}
   glEndList();

   // Stem
   glNewList(STEM,GL_COMPILE);
   	glColor3f(0,0.4,0);
	   	glPushMatrix();
	   	glTranslatef(0,0,-1);
		   	// Base
		   	glBegin(GL_TRIANGLE_FAN);
		   		glVertex3f(0,0,0);
		   		for(float angle = 0; angle <= 360; angle+=0.1) {
		   			glVertex3f(s_radius*cos(toRad(angle)),0,s_radius*sin(toRad(angle)));
		   		}
		   	glEnd();

		   	// Top
		   	glBegin(GL_TRIANGLE_FAN);
		   		glVertex3f(0,30,0);
		   		for(float angle = 0; angle <= 360; angle+=0.1) {
		   			glVertex3f((s_radius-0.5)*cos(toRad(angle)),30,(s_radius-0.5)*sin(toRad(angle)));
		   		}
		   	glEnd();

		   	// Body
		   	glBegin(GL_QUAD_STRIP);
		   		for(float angle = 0; angle <= 360; angle+=0.1) {
		   			glVertex3f((s_radius-0.5)*cos(toRad(angle)),30,(s_radius-0.5)*sin(toRad(angle)));
		   			glVertex3f(s_radius*cos(toRad(angle)),0,s_radius*sin(toRad(angle)));
		   		}
		   	glEnd();
		glPopMatrix();
   glEndList();

   // Petal
   glNewList(PETAL,GL_COMPILE);
   		glColor3f(1,1,1);
   		glPushMatrix();
   			glScalef(3,1,0.5);
   			glutSolidSphere(2.5,50,50);
   		glPopMatrix();
   glEndList();
}

// Rendering
void draw_scene() {
	glInitNames();
	glPushName(0);

	// Hill
	glPushMatrix();
		glCallList(HILL);
	glPopMatrix();

	// Flower
	glPushMatrix();
		glCallList(STEM);
			glTranslatef(0,30,0);
			glScalef(1,1,0.3);
			glColor3f(1,0.5,0);
			glutSolidSphere(6,50,50);
				for(int i=0; i<n_petals; i++) {
					p_radius = (360/n_petals)*i;
					glPushMatrix();
						glRotatef(p_radius,0,0,1);
						glTranslatef(rad_dist,0,3);
						glTranslatef(p_trasly,0,0);
						glRotatef(-20,0,1,0);
						glLoadName(i);
						glCallList(PETAL);
					glPopMatrix();
				}
	glPopMatrix();

	// Leaf
	glPushMatrix();
		glColor3f(0,0.4,0);
		glTranslatef(4,12.5,-1);
		glRotatef(10,0,0,1);
		glScalef(2,1,0.3);
		glutSolidSphere(2,50,50);
	glPopMatrix();
}

void display (void) {
   glClearColor(0,1,1,1);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
  	glTranslatef(0,-20,-100);
   	glRotatef(rotx,1,0,0);
   	glRotatef(roty,0,1,0);
   		draw_scene();
   glPopMatrix();

   glutSwapBuffers();
}

void process_selection(int x, int y) {
	// Buffer di selezione
	GLuint selectBuffer[64];

	GLfloat fAspect;

	// Contatore Hits e Viewport
	GLint hits, viewport[4];

	// Selezione buffer
	glSelectBuffer(64,selectBuffer);

	// Calcolo dimensioni della viewport
	glGetIntegerv(GL_VIEWPORT,viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Cambio modalità di rendering
	glRenderMode(GL_SELECT);
	glLoadIdentity();

	// Modifichiamo il Viewing Volume con gluPickMatrix
	gluPickMatrix(x,viewport[3]-y,2,2,viewport);

	// Applico una proiezione prospettica
	fAspect = (GLfloat)viewport[2]/(GLfloat)viewport[3];
	gluPerspective(65.0f,fAspect,1.0,800.0f);

	display();

	// Ripristino la matrice di proiezione

	// Calcolo del numero di hits
	hits = glRenderMode(GL_RENDER);
	if(hits >= 1) {
		anim = !anim;
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

void mouse_callback(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		process_selection(x,y);
}

void idle() {
	if(anim) {
		p_trasly -= -0.2;
		p_rotx += 5;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 'a':
         roty+=1.0;
         break;
      case 'd':
         roty-=1.0;
         break;
	  case 'w':
         rotx+=1.0;
         break;
      case 's':
         rotx-=1.0;
         break;
      case '+':
      	 if(n_petals < 16)
      	 	n_petals++;
      	 break;
      case '-':
      	 if(n_petals > 4)
      	 	n_petals--;
      	 break;
   }
   glutPostRedisplay();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1200, 700);
   glutCreateWindow ("Flowers");
   init();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutMouseFunc(mouse_callback);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0; 
}