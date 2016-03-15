#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>

#define PI 3.1415

#define BUILD 1
#define SPIN 2
#define PERSON 3


// Light parameters

GLfloat global_ambient[] = {0.3f,0.3f,0.3f,1.0f};

GLfloat dir_ambient[] = {0.8f,0.8f,0.8f,1.0f};
GLfloat dir_diffuse[] = {0.8f,0.8f,0.8f,1.0f};
GLfloat dir_specular[] = {1.0f,1.0f,1.0f,1.0f};

GLfloat dir_position[] = {0.0f,1.0f,1.0f,0.0f};

// Color Material - Specular
GLfloat mat_specular[] = {0.6,0.6,0.6,1.0};

// Scene Rotation on x axis and y axis
GLfloat x_rot = 0;
GLfloat y_rot = 0;

// Coordinates x,y
GLfloat spin_angle = 0;
GLfloat y_center;

// Radius
GLfloat radius = 5;

// Disk Rotation
GLfloat y_disk = 0;

// Number of people
GLint n_people = 8;
GLfloat p_radius;
GLfloat rad_dist = 4;
GLint verso = 1;

// Animation bool
bool stop_anim = false;

void init() {
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Setting lights
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glLightfv(GL_LIGHT0,GL_AMBIENT, dir_ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE, dir_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR, dir_specular);

	glLightfv(GL_LIGHT0,GL_POSITION,dir_position);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMateriali(GL_FRONT,GL_SHININESS,40); 

	glEnable(GL_NORMALIZE);
}

void init_lists() {
	// Build
	glNewList(BUILD,GL_COMPILE);
		glColor3f(0,0.6,0);

		// Bottom face
		glBegin(GL_QUADS);
			glVertex3f(25,0,2.5);
			glVertex3f(-25,0,2.5);
			glVertex3f(-25,0,-2.5);
			glVertex3f(25,0,-2.5);
		glEnd();

		// Left Face
		glBegin(GL_QUADS);
			glVertex3f(-25,30,-2.5);
			glVertex3f(-25,30,2.5);
			glVertex3f(-25,0,2.5);
			glVertex3f(-25,0,-2.5);
		glEnd();

		// Right Face
		glBegin(GL_QUADS);
			glVertex3f(25,30,2.5);
			glVertex3f(25,30,-2.5);
			glVertex3f(25,0,-2.5);
			glVertex3f(25,0,2.5);
		glEnd();

		
 	// 	Front face
	y_center = sin(((270*PI)/180))*25+30;
 	for(float angle = 180; angle < 360; angle += 0.1f) {
 		glBegin(GL_QUADS);
 			float rad  = angle*PI/180;
 			float rad1 = ((angle+0.1)*PI)/180;
 			glVertex3f(cos(rad)*25,(sin(rad)*25)+30,2.5);
 			glVertex3f(cos(rad)*25,0,2.5);
 			glVertex3f(cos(rad1)*25,0,2.5);
 			glVertex3f(cos(rad1)*25,(sin(rad1)*25)+30,2.5);
 		glEnd();

 		// Back Face
 		glBegin(GL_QUADS);
 			glVertex3f(cos(rad)*25,(sin(rad)*25)+30,-2.5);
 			glVertex3f(cos(rad)*25,0,-2.5);
 			glVertex3f(cos(rad1)*25,0,-2.5);
 			glVertex3f(cos(rad1)*25,(sin(rad1)*25)+30,-2.5);
 		glEnd();

 		// Central top face
 		glBegin(GL_QUADS);
 			glVertex3f(cos(rad)*25,(sin(rad)*25)+30,-2.5);
 			glVertex3f(cos(rad)*25,(sin(rad)*25)+30,2.5);
 			glVertex3f(cos(rad1)*25,(sin(rad1)*25)+30,+2.5);
			glVertex3f(cos(rad1)*25,(sin(rad1)*25)+30,-2.5);
 		glEnd();
 	}
	glEndList();

	// SPIN
	glNewList(SPIN,GL_COMPILE);
		glColor3f(0.6,0,0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0,1,0);
			for(float angle = 0; angle <= 360; angle+=0.1f) {
				// Top circle	
				glVertex3f(radius*cos((angle*PI)/180),1,radius*sin((angle*PI)/180));
			}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0,0,0);
			for(float angle = 0; angle <= 360; angle+=0.1f) {
				// Top circle	
				glVertex3f(radius*cos((angle*PI)/180),0,radius*sin((angle*PI)/180));
			}
			glEnd();

			glBegin(GL_QUAD_STRIP);
				for(float angle = 0; angle <= 360; angle+=0.1f) {
				glVertex3f(radius*cos((angle*PI)/180),1,radius*sin((angle*PI)/180));
				glVertex3f(radius*cos((angle*PI)/180),0,radius*sin((angle*PI)/180));
				glVertex3f(radius*cos(((angle*PI)/180)+0.1),0,radius*sin(((angle*PI)/180)+0.1));
				glVertex3f(radius*cos(((angle*PI)/180)+0.1),1,radius*sin(((angle*PI)/180)+0.1));
			}
			glEnd();
	glEndList();

	// Puppets
	glNewList(PERSON,GL_COMPILE);
		glColor3f(0,0,1);
		glPushMatrix();
			glTranslatef(0,2.5,0);
			glutSolidSphere(0.5,50,50);
		glPopMatrix();
		glColor3f(1,1,0);
		glPushMatrix();
			glRotatef(-90,1,0,0);
			glutSolidCone(0.5,2,50,50);
		glPopMatrix(); 
	glEndList();
}

void draw_scene() {
	glShadeModel(GL_SMOOTH);

	glDeleteLists(SPIN,1);
	init_lists();	

	glCallList(BUILD);

		glPushMatrix();

		glTranslatef(0,y_center+25,0);
		glRotatef(85*sin((spin_angle*PI)/180),0,0,1);
		glTranslatef(0,y_center-30,0);
			glPushMatrix();
				glTranslatef(0,1.5,0);
				glutSolidCube(3);
			glPopMatrix();
		
			glPushMatrix();
				glRotatef(y_disk,0,1,0);
				glTranslatef(0,3,0);
				glCallList(SPIN);
			glPopMatrix();

			glPushMatrix();
				glRotatef(y_disk,0,1,0);
				glTranslatef(0,4,0);
				for(int i=0; i < n_people; i++) {
					p_radius = (360.0/n_people)*i;
					glPushMatrix();
						glRotatef(p_radius,0,1,0);
						glTranslatef(rad_dist,0,0);
						glRotatef(-90,0,1,0);
						glCallList(PERSON);
					glPopMatrix();	
				}
			glPopMatrix();
		glPopMatrix();
}

void idle() {
	if(!stop_anim || (int)spin_angle%180!= 0) {
		y_disk += 5;	
		spin_angle += 1;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char k, int x, int y) {
	switch(k) {
		case 'a': 
					y_rot += 1.5f;
					break;
		case 'd': 	        y_rot -= 1.5f;
					break;
		case 'w':
					x_rot += 1.5f;
					break;
		case 's':
					x_rot -= 1.5f;
					break;
		case '+':
					if(radius < 10) {
						radius += 0.1f;
						rad_dist += 0.1f;
					}

					break;
		case '-':
					if(radius > 3) {
						radius -= 0.1f;
						rad_dist -= 0.1f;
					}
					break;
		case 'o':
					if(n_people < 10)
						n_people++;
					break;
		case 'p':
					if(n_people > 4)
						n_people--;
					break;
		case 'b':
					stop_anim = !stop_anim;
					break;
	}

	glutPostRedisplay();
}

void display() {
	glClearColor(0.2,0.2,0.2,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glRotatef(x_rot,1,0,0);
		glRotatef(y_rot,0,1,0);
		glLightfv(GL_LIGHT0,GL_POSITION,dir_position);
		draw_scene();
	glPopMatrix();

	glutSwapBuffers();
}

// Projection
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
    glTranslatef(0,-10,-70.0f);
}

// Main
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
