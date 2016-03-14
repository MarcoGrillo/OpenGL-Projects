#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>

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
}

void draw_scene() {

}

void display() {
	glClearColor(0,0.6,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glRotatef(x_rot,1,0,0);
		glRotatef(y_rot,0,1,0);
		glLightfv(GL_LIGHT0,GL_POSITION,dir_position);
		draw_scene();
	glPopMatrix();

	glutSwapBuffers();
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
    glTranslatef(0,0,-200.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(1000,600);
	glutCreateWindow("Scena");
	glutDisplayFunc(display);
	glutMouseFunc(mouse_callback);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}