#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <climits>

#define SCENA 5
#define CHAIN 1
#define SEAT 3
#define HILL 2
#define ASTA 4

#define PI 3.1415

// Rotazioni scena
GLfloat x_rot = 0;
GLfloat y_rot = 0;

// Fattore alfa
GLfloat alfa = 0.01f;

// Angolo altalena
GLfloat angle = 0;
GLfloat verso = 0.8;

// Luce ambientale globale
GLfloat global_ambient[] = {0.1f,0.1f,0.1f,1.0f};

// Luce posizionale
GLfloat dir_ambient[] = {0.3f,0.3f,0.3f,1.0f};
GLfloat dir_diffuse[] = {0.8f,0.8f,0.8f,1.0f};
GLfloat dir_specular[] = {1.0f,1.0f,1.0f,1.0f};

// Posizione luce
GLfloat dir_position[] = {80.0f,80.0f,80.0f,1.0f};

// Color Material - Specular
GLfloat mat_specular[] = {0.6,0.6,0.6,1.0};

// Trigger animazione
bool anim_trigger = false;


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

	glNewList(HILL,GL_COMPILE);
		for(float x = -50; x < 50; x+=0.5f) {
			for(float z = -50; z < 50; z+=0.5f) {
				glColor3f(0,1,0);
				glBegin(GL_QUADS);
					glVertex3f(x,-alfa*(pow(x,2)+pow(z,2)),z);
					glVertex3f(x,-alfa*(pow(x,2)+pow(z+0.5,2)),z+0.5);
					glVertex3f(x+0.5,-alfa*(pow(x+0.5,2)+pow(z+0.5,2)),z+0.5);
					glVertex3f(x+0.5,-alfa*(pow(x+0.5,2)+pow(z,2)),z);
				glEnd();
			}
		}
	glEndList();

	glNewList(CHAIN,GL_COMPILE);
		glPushMatrix();
			glColor3f(0.5,0.5,0.5);
			glutSolidTorus(0.5,2,60,60);
		glPopMatrix();
	glEndList();
}

void draw_scene() {

	glShadeModel(GL_SMOOTH);
	glInitNames();
	glPushName(SCENA);

	glPushMatrix();
		glPushName(HILL);
		glCallList(HILL);
	glPopMatrix();

	glPushMatrix();
		glLoadName(CHAIN);
		glPushMatrix();
			glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+46.5,0);
			glRotatef(40*sin(angle),1,0,0);
			glTranslatef(0,-(-alfa*(pow(-19,2)+pow(0,2))+46.5),0);
			for(int i=0; i<10; i++) {
			glPushMatrix();
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+ 46.5,0);
				glPushMatrix();
					glTranslatef(-8,-i*3,0);
					if(i%2==0)
						glRotatef(90,0,1,0);
					glCallList(CHAIN);
				glPopMatrix();
			glPopMatrix();
			}

			
			for(int i=0; i<10; i++) {
			glPushMatrix();
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+ 46.5,0);
				glPushMatrix();
					glTranslatef(8,-i*3,0);
					if(i%2==0)
						glRotatef(90,0,1,0);
					glCallList(CHAIN);
				glPopMatrix();
			glPopMatrix();
			}

			glColor3f(0.4,0.4,0.4);

			glColor3f(0.8,0.3,0);
			// Seggiolino
			glPushMatrix();
				glLoadName(SEAT);
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+16,0);
				glScalef(16,2,5);
				glutSolidCube(1);
			glPopMatrix();

			glColor3f(1,0,0);
			glPushMatrix();
				glLoadName(SEAT);
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+17,0);
				glRotatef(-90,1,0,0);
				glutSolidCone(2.5,8,60,60);
			glPopMatrix();
		glPopMatrix();

			glColor3f(0.3,0.2,0);
			// Asta sx
			glPushMatrix();
				glLoadName(ASTA);
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2)),0);
				glTranslatef(-17,25,0);
				glScalef(4,50,4);
				glutSolidCube(1);
			glPopMatrix();

			glColor3f(0.3,0.2,0);
			// Asta dx
			glPushMatrix();
				glLoadName(ASTA);
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2)),0);
				glTranslatef(17,25,0);
				glScalef(4,50,4);
				glutSolidCube(1);
			glPopMatrix();

			glColor3f(0.5,0.5,0.5);
			// Asta centrale
			glPushMatrix();
				glTranslatef(0,-alfa*(pow(-19,2)+pow(0,2))+48,0);
				glBegin(GL_LINES);
					glVertex2f(-16,0);
					glVertex2f(16,0);
				glEnd();
			glPopMatrix();
	glPopMatrix();
}

void display() {
	glClearColor(0,0.6,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glRotatef(x_rot,1,0,0);
		glRotatef(y_rot,0,1,0);
		glLightfv(GL_LIGHT0,GL_POSITION,dir_position);
		draw_scene();
	glPopMatrix();

	glutSwapBuffers();
}

void process_zObject(GLuint *selectBuffer, GLint hits) {
	int id = 0;
	GLuint z_min = UINT_MAX; // Sparo al massimo la z
	GLuint id_min = INT_MIN; // Sparo al minimo l'id

	for(int i=0; i<hits; i++) {
		for(int j=id; j<=(id+2+selectBuffer[id]); j++) {
			std::cout << selectBuffer[j] << std::endl;
		}

		std::cout << std::endl;
		// Se il valore di z nel buffer
		if(selectBuffer[id+1] < z_min) {
			// Prendere il valore della z_min nel buffer
			z_min = selectBuffer[id+1];
			id_min = selectBuffer[id+2+selectBuffer[id]];
		}

		id += selectBuffer[id]+3;
	}

	std::cout << "Selected object: " << id_min << std::endl;

	glutPostRedisplay();
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
	gluPickMatrix(x,viewport[3]-y,4,4,viewport);

	// Applico una proiezione prospettica
	fAspect = (GLfloat)viewport[2]/(GLfloat)viewport[3];
	gluPerspective(45.0f,fAspect,1.0,300.0f);

	display();

	// Ripristino la matrice di proiezione

	// Calcolo del numero di hits
	hits = glRenderMode(GL_RENDER);
	if(hits >= 1) {
		process_zObject(selectBuffer,hits);
		anim_trigger = !anim_trigger;
	}
	else
		std::cout << "Hai cliccato spazio vuoto" << std::endl;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void mouse_callback(int button,int state, int x,int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		process_selection(x,y);
}

void idle() {
	
	if(anim_trigger) {
		angle += 0.2*verso;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char k, int x, int y) {
	switch(k) {
		case 'a': 
					y_rot -= 1.0f;
					break;
		case 'd': 	y_rot += 1.0f;
					break;
		case 'w':
					x_rot += 1.0f;
					break;
		case 's':
					x_rot -= 1.0f;
					break;
		case '+':	
					glDeleteLists(HILL,1);
					glDeleteLists(CHAIN,1);
					alfa += 0.0001f;
					init();
					break;
		case '-': 	
					glDeleteLists(HILL,1);
					glDeleteLists(CHAIN,1);
					alfa -= 0.0001f;
					init();
					break;
	}

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