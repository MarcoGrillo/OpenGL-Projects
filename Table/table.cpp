#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#define GAMBA 1

// Rotazione scena
GLfloat y_rot = 0;

// Traslazione tavolo
GLfloat x_tav = 0;
GLfloat z_tav = 0;

// Traslazione trottola
GLfloat x_bey = 0;
GLfloat z_bey = 0;

// Rotazione trottola
GLfloat y_rot_b = 0;

// Luce ambientale globale
GLfloat global_ambient[] = {0.3f,0.3f,0.3f,1.0f};

// Luce direzionale
GLfloat dir_ambient[] = {0.5f,0.5f,0.5f,1.0f};
GLfloat dir_diffuse[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat dir_specular[] = {1.0f,1.0f,1.0f,1.0f};

// Posizione luce
GLfloat dir_position[] = {1.0f,0.2f,0.0,0.0f};

// Color Material - Specular
GLfloat mat_specular[] = {0.6,0.6,0.6,1.0};


void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glLightfv(GL_LIGHT0,GL_AMBIENT, dir_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, dir_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR, dir_specular);

	glLightfv(GL_LIGHT0,GL_POSITION,dir_position);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMateriali(GL_FRONT,GL_SHININESS,30);

	glEnable(GL_NORMALIZE);

	glNewList(GAMBA,GL_COMPILE);
		glColor3f(0.4,0.2,0);
		glScalef(2,12,2);
		glutSolidCube(1);
	glEndList();
}

void draw_scene() {

	glShadeModel(GL_SMOOTH);
		glPushMatrix();

		glTranslatef(x_tav,0,z_tav);

			// Trottola
			glPushMatrix();
				glColor3f(0,0,1);
				glTranslatef(x_bey,17,z_bey);
				glRotatef(y_rot_b,0,1,0);
				glRotatef(90,1,0,0);
				glutSolidCone(1,3,50,50);
			glPopMatrix();

			glPushMatrix();
				glColor3f(0,1,0);
				glTranslatef(x_bey,18,z_bey);
				glScalef(0.5,2,0.5);
				glRotatef(y_rot_b,0,1,0);
				glutSolidCube(1);
			glPopMatrix();


		// Piano del tavolo
		glPushMatrix();
			glColor3f(0.4,0.2,0);
			glTranslatef(0,13,0);
			glScalef(22,2,22);
			glutSolidCube(1);
		glPopMatrix();

		// Gambe
		glPushMatrix();
			glTranslatef(10,6.0,-10);
			glCallList(GAMBA);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(-10,6.0,-10);
			glCallList(GAMBA);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(10,6.0,10);
			glCallList(GAMBA);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-10,6.0,10);
			glCallList(GAMBA);
		glPopMatrix();
	glPopMatrix();

	// Pavimento
	glDisable(GL_LIGHTING);
	glColor3f(0.5f,0.5f,0.5f);
	glRotatef(-90,1,0,0);
	glRectf(-80,-80,80,80);
	glEnable(GL_LIGHTING);
}

void display() {
	glClearColor(0,0,0.3,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glRotatef(y_rot,0,1,0);
		draw_scene();
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char k, int x, int y) {
	switch(k) {
		case 'z': 
					y_rot += 0.5f;
					break;
		case 'x': 	y_rot -= 0.5f;
					break;
		case 'a':
					if(x_bey > -11.0f)
						x_bey -= 0.5f;
					break;
		case 'd':
					if(x_bey < 11.0f)
						x_bey += 0.5f;
					break;
		case 'w':
					if(z_bey > -11.0f)
						z_bey -= 0.5f;
					break;
		case 's':	
					if(z_bey < 11.0f)
						z_bey += 0.5f;
					break;
		case 'j':   
					if(x_tav > -69.0f)
						x_tav -= 0.5f;
					break;
		case 'l':
					if(x_tav < 69.0f)
						x_tav += 0.5f;
					break;
		case 'i':
					if(z_tav > -69.0f)
						z_tav -= 0.5f;
					break;
		case 'k':
					if(z_tav < 69.0f)
						z_tav += 0.5f;
					break;
	}

	glutPostRedisplay();
}

void idle() {
	y_rot_b += 0.5;
	glutPostRedisplay();
}

void reshape(int w, int h){
	GLfloat fAspect;
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, fAspect, 1.0f, 800.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,-20,-80.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(1000,600);
	glutCreateWindow("Scena");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}