#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#define GAP  20 /* gap between subwindows, trick learned from class */
//static int verticesTotal = 0, facesTotal = 0;
GLuint window, screen1, screen2, screen3, screen4, screen5, screen6;
GLuint sub_width = 150, sub_height = 150;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

static int screen_width = 3*sub_width+4*GAP, screen_height = 2*sub_height+3*GAP;
static const float TEAPOT_SIZE = 0.4;

GLdouble projection[16], modelview[16], inverse[16];

void init() {
	//glViewport(0,0,screen_width,screen_height);//Setup for future reference
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	//glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glPushMatrix();
	/*glShadeModel (GL_FLAT);*/
}

//Utility function for reference from Prof. Lucy Yang's code on projections
void identityD(GLdouble m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

void myTranslatef(float x, float y, float z) {
	GLfloat  m[16] = {0.0f};
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = x;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = y;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = z;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
	glMultMatrixf(m);
}

//Utility function for reference from Prof. Lucy Yang's code on projections
GLboolean invert(GLdouble src[16], GLdouble inverse[16])
{
	double t;
	int i, j, k, swap;
	GLdouble tmp[4][4];
	identityD(inverse);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			tmp[i][j] = src[i * 4 + j];
		}
	}
	for (i = 0; i < 4; i++) {
		/* look for largest element in column. */
		swap = i;
		for (j = i + 1; j < 4; j++) {
			if (fabs(tmp[j][i]) > fabs(tmp[i][i])) {
				swap = j;
			}
		}
		if (swap != i) {
			/* swap rows. */
			for (k = 0; k < 4; k++) {
				t = tmp[i][k];
				tmp[i][k] = tmp[swap][k];
				tmp[swap][k] = t;

				t = inverse[i * 4 + k];
				inverse[i * 4 + k] = inverse[swap * 4 + k];
				inverse[swap * 4 + k] = t;
			}
		}
		if (tmp[i][i] == 0) {
			/* no non-zero pivot.  the matrix is singular, which
			shouldn't happen.  This means the user gave us a bad
			matrix. */
			return GL_FALSE;
		}
		t = tmp[i][i];
		for (k = 0; k < 4; k++) {
			tmp[i][k] /= t;
			inverse[i * 4 + k] /= t;
		}
		for (j = 0; j < 4; j++) {
			if (j != i) {
				t = tmp[j][i];
				for (k = 0; k < 4; k++) {
					tmp[j][k] -= tmp[i][k] * t;
					inverse[j * 4 + k] -= inverse[i * 4 + k] * t;
				}
			}
		}
	}
	return GL_TRUE;
}

void drawstr(GLuint x, GLuint y, char* format, ...) 
{ 	//String draw functions retrieved from example code by Prof. Lucy Yang
	//(Thank you!)
	va_list args;
	char buffer[255], *s;
	va_start(args, format);
	vsprintf_s(buffer, 30, format, args);
	va_end(args);
	glRasterPos2i(x, y);
	for (s = buffer; *s; s++)
		glutBitmapCharacter(font_style, *s);
}

void drawTeapot() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3d(0.9, 0.4, 0.1);
	glutSolidTeapot(TEAPOT_SIZE);
}

void main_keyboard (unsigned char key, int x, int y)
{ /*will use in future assignments for pitch, yaw, roll*/
   switch (key) {
      case 'w':   /*  s key rotates at shoulder  */
         //shoulder = (shoulder + 5) % 360;
         //glutPostRedisplay();
         break;
	  case '1':
		  //teaPotMultiples = 1;
		  //elbow = (elbow - 5) % 360;
		  glutPostRedisplay();
		  break;
      default:
         break;
   }
   /*glPopMatrix();
   glLoadIdentity();
   glTranslatef(currentCameraX,currentCameraY,0.f);
   glPushMatrix();*/
}

void main_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	sub_width = (width - GAP * 4) / 3.0;
	sub_height = (height - GAP * 3) / 2.0;
	glutSetWindow(screen1);
	glutPositionWindow(GAP, GAP);
	glutReshapeWindow(sub_width, sub_height);
	glutSetWindow(screen2);
	glutPositionWindow(GAP + sub_width + GAP, GAP);
	glutReshapeWindow(sub_width, sub_height);
	glutSetWindow(screen3);
	glutPositionWindow(GAP + 2 * sub_width + 2 * GAP, GAP);
	glutReshapeWindow(sub_width, sub_height);
	glutSetWindow(screen4);
	glutPositionWindow(GAP, GAP + sub_height + GAP);
	glutReshapeWindow(sub_width, sub_height);
	glutSetWindow(screen5);
	glutPositionWindow(GAP + sub_width + GAP, GAP + sub_height + GAP);
	glutReshapeWindow(sub_width, sub_height);
	glutSetWindow(screen6);
	glutPositionWindow(GAP + 2 * sub_width + 2 * GAP, GAP + sub_height + GAP);
	glutReshapeWindow(sub_width, sub_height);
}

void main_display(void)
{
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//String draw functions retrieved from example code by Prof. Lucy Yang
	//(Thank you!)
	glColor3d(0.5, 0.0, 0.7); 
	drawstr(GAP, GAP - 5, "Rot 30Y,Rot 45Z,Trans");
	drawstr(GAP * 2 + sub_width, GAP - 5, "Rot 45Z,Rot 30Y,Trans");
	drawstr(GAP * 3 + 2*sub_width, GAP - 5, "Trans,Rot 30Y,Rot 45Z");
	drawstr(GAP, GAP*2 + sub_height - 5, "Trans,Rot 45Z,Rot 30Y");
	drawstr(GAP * 2 + sub_width, GAP*2 + sub_height - 5, "Rot 30Y,Trans,Rot 45Z");
	drawstr(GAP * 3 + 2*sub_width, GAP*2 + sub_height - 5, "Rot 45Z,Trans,Rot 30Y");
	glColor3d(0.0, 0.0, 0.0);
	glutSwapBuffers();

	/*

	else if (glutGetWindow() == screen6) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		*/
}

void screen_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (float)width / height, 0.5, 8.0);
	//glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();	
	//gluLookAt(3,-3,3,-3,3,-3,3,-3,3);
	if (glutGetWindow() == screen1) {
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, -0.5);
		//myTranslatef(0.0, 0.0, -0.5);
	}
	else if (glutGetWindow() == screen2) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5);
	}
	else if (glutGetWindow() == screen3) {
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glRotatef(45.0, 0.0, 0.0, 1.0);
	}
	else if (glutGetWindow() == screen4) {
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
	}
	else if (glutGetWindow() == screen5) {
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(45.0, 0.0, 0.0, 1.0);
	}
	else if (glutGetWindow() == screen6) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(30.0, 0.0, 1.0, 0.0);
	}
	drawTeapot();
	glPopMatrix();
	glutSwapBuffers();
	//glClearColor(0.2, 0.2, 0.2, 0.0);
}

void screen_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(0.5, 0.0, 0.7);
	glFrustum(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	if (glutGetWindow() == screen1) {
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, -0.5);
	}
	else if (glutGetWindow() == screen2) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5);
	}
	else if (glutGetWindow() == screen3) {
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glRotatef(45.0, 0.0, 0.0, 1.0);
	}
	else if (glutGetWindow() == screen4) {
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
	}
	else if (glutGetWindow() == screen5) {
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(45.0, 0.0, 0.0, 1.0);
	}
	else if (glutGetWindow() == screen6) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, -0.5);
		glRotatef(30.0, 0.0, 1.0, 0.0);
	}
	drawTeapot();
	glPopMatrix();
	glutSwapBuffers();
}

void redisplay_all(void)
{
	glutSetWindow(screen1);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
	glutSetWindow(screen2);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
	glutSetWindow(screen3);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
	glutSetWindow(screen4);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
	glutSetWindow(screen5);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
	glutSetWindow(screen6);
	screen_reshape(sub_width, sub_height);
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
   glutInitWindowSize(screen_width, screen_height);
   glutInitWindowPosition(50, 50);
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //GLU_DOUBLE used when movement's involved.
   init();

   window = glutCreateWindow("Question 1 Assignment 2");
   glutReshapeFunc(main_reshape);
   glutDisplayFunc(main_display);
   glutKeyboardFunc(main_keyboard);
   
   screen1 = glutCreateSubWindow(window, GAP, GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   screen2 = glutCreateSubWindow(window, GAP + 150 + GAP, GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   screen3 = glutCreateSubWindow(window, GAP + 150 + GAP + 150 + GAP, GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   screen4 = glutCreateSubWindow(window, GAP, GAP + 150 + GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   screen5 = glutCreateSubWindow(window, GAP + 150 + GAP, GAP + 150 + GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   screen6 = glutCreateSubWindow(window, GAP + 150 + GAP + 150 + GAP, GAP + 150 + GAP, 150, 150);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   redisplay_all();
   
   glutMainLoop();
   return 0;
}
