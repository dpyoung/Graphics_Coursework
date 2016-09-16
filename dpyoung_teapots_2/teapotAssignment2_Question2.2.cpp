#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h> 
using namespace std;

#define GAP  20 /* gap between subwindows, trick learned from class */
#define PI 3.14159265
//static int verticesTotal = 0, facesTotal = 0;
GLuint window, screen1/*, screen2, screen3, screen4, screen5, screen6*/;
GLuint sub_width = 150, sub_height = 150;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;


static int screen_width = 2*sub_width+2*GAP, screen_height = 2*sub_height+2*GAP;
static const double TEAPOT_SIZE = 0.4;

GLdouble projection[16], modelview[16], inverse[16];
GLdouble eyePosition[3] = { 0.0, 0.0, 1.0 };
static double N[3], U[3], V[3];
static double angleP = 0.0, angleY = 0.0, angleR = 0.0;

void init() {
	//glViewport(0,0,screen_width,screen_height);//Setup for future reference
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//Utility function for reference from Prof. Lucy Yang's code on projections
void identityD(GLdouble m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

/*My Translation function, translated from lecture materials*/
void myTranslatef(float x, float y, float z) {
	GLfloat  m[16] = {0.0f};
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = x;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = y;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = z;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
	glMultMatrixf(m);
}

/*My Rotation function, translated from lecture materials*/
void myRotatef(float degrees, float x, float y, float z) {
	GLdouble  m[16] = { 0.0f };
	/*x rot*/
	m[0 + 4 * 0] = 1;	m[0 + 4 * 1] = 0;						m[0 + 4 * 2] = 0;						m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0;	m[1 + 4 * 1] = cos(x*degrees*PI/180);	m[1 + 4 * 2] = -sin(x*degrees*PI/180);	m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0;	m[2 + 4 * 1] = sin(x*degrees*PI/180);	m[2 + 4 * 2] = cos(x*degrees*PI/180);	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;	m[3 + 4 * 1] = 0;						m[3 + 4 * 2] = 0;						m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
	/*y rot*/
	m[0 + 4 * 0] = cos(y*degrees*PI/180);	m[0 + 4 * 1] = 0;	m[0 + 4 * 2] = sin(y*degrees*PI/180);	m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0;						m[1 + 4 * 1] = 1;	m[1 + 4 * 2] = 0;						m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = -sin(y*degrees*PI/180);	m[2 + 4 * 1] = 0;	m[2 + 4 * 2] = cos(y*degrees*PI/180);	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;						m[3 + 4 * 1] = 0;	m[3 + 4 * 2] = 0;						m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
	/*z rot*/
	m[0 + 4 * 0] = cos(z*degrees*PI/180);	m[0 + 4 * 1] = -sin(z*degrees*PI/180);	m[0 + 4 * 2] = 0;	m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = sin(z*degrees*PI/180);	m[1 + 4 * 1] = cos(z*degrees*PI/180);	m[1 + 4 * 2] = 0;	m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0;						m[2 + 4 * 1] = 0;						m[2 + 4 * 2] = 1;	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;						m[3 + 4 * 1] = 0;						m[3 + 4 * 2] = 0;	m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
}

//This code (translated from the class PowerPoint lecture) seems to take in a far variable and not use it? O_o
void myPerspective(double fov, double aspect, double neeer, double far) {
	double f = cos((fov/2)*PI / 180) / sin((fov/2)*PI / 180);
	GLdouble  m[16] = { 0.0f };
	//GLdouble m[16] = { 0.0 };
	m[0 + 4 * 0] = f / aspect;	m[0 + 4 * 1] = 0;	m[0 + 4 * 2] = 0;							m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0;			m[1 + 4 * 1] = f;	m[1 + 4 * 2] = 0;							m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0;			m[2 + 4 * 1] = 0;	m[2 + 4 * 2] = ((f + neeer)/(neeer - f));	m[2 + 4 * 3] = ((2 * f * neeer)/(neeer - f));
	m[3 + 4 * 0] = 0;			m[3 + 4 * 1] = 0;	m[3 + 4 * 2] = -1;							m[3 + 4 * 3] = 0;
	glMultMatrixd(m);
}

void normalize(double * vect) {
	double length = sqrt(vect[0]*vect[0] + vect[1]*vect[1] + vect[2]*vect[2]);
	vect[0] = length*vect[0] / abs(length);
	vect[1] = length*vect[1] / abs(length);
	vect[2] = length*vect[2] / abs(length);
}

//myLookat()
void myLookat(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, 
	double upX, double upY, double upZ) 
	{
	//double f = sin((fov / 2)*PI / 180) / cos((fov / 2)*PI / 180);
	GLdouble  m[16] = { 0.0f };
	N[0] = eyeX - centerX;
	N[1] = eyeY - centerY;
	N[2] = eyeZ - centerZ;
	normalize(N);
	//U = Vup X n
	U[0] = upY*N[2] - upZ*N[1];
	U[1] = upZ*N[0] - upX*N[2];
	U[2] = upX*N[1] - upY*N[0];
	normalize(U);
	V[0] = N[1]*U[2] - N[2]*U[1];
	V[1] = N[2]*U[0] - N[0]*U[2];
	V[2] = N[0]*U[1] - N[1]*U[0];
	/*Already normalized*/

	/*?*/
	float csP = cos(PI / 180 * angleP);
	float snP = sin(PI / 180 * angleP);
	double V2[3] = { V[0], V[1], V[2] }; // remember old u
	V[0] = (csP*V2[0] - snP*N[0]);
	V[1] = (csP*V2[1] - snP*N[1]);
	V[2] = (csP*V2[2] - snP*N[2]);
	N[0] = snP*V2[0] + csP*N[0];
	N[1] = snP*V2[1] + csP*N[1];
	N[2] = snP*V2[2] + csP*N[2];

	float csY = cos(PI / 180 * angleY);
	float snY = sin(PI / 180 * angleY);
	double N2[3] = { N[0], N[1], N[2] }; // remember old u
	N[0] = (csY*N2[0] - snY*U[0]);
	N[1] = (csY*N2[1] - snY*U[1]);
	N[2] = (csY*N2[2] - snY*U[2]);
	U[0] = snY*N2[0] + csY*U[0];
	U[1] = snY*N2[1] + csY*U[1];
	U[2] = snY*N2[2] + csY*U[2];

	float csR = cos(PI / 180 * angleR);
	float snR = sin(PI / 180 * angleR);
	double U2[3] = { U[0], U[1], U[2] }; // remember old u
	U[0] = (csR*U2[0] - snR*V[0]);
	U[1] = (csR*U2[1] - snR*V[1]);
	U[2] = (csR*U2[2] - snR*V[2]);
	V[0] = snR*U2[0] + csR*V[0];
	V[1] = snR*U2[1] + csR*V[1];
	V[2] = snR*U2[2] + csR*V[2];

	m[0 + 4 * 0] = U[0];		m[0 + 4 * 1] = U[1];	m[0 + 4 * 2] = U[2];	m[0 + 4 * 3] = -(eyeX*U[0]+eyeY*U[1]+eyeZ*U[2]);
	m[1 + 4 * 0] = V[0];		m[1 + 4 * 1] = V[1];	m[1 + 4 * 2] = V[2];	m[1 + 4 * 3] = -(eyeX*V[0] + eyeY*V[1] + eyeZ*V[2]);
	m[2 + 4 * 0] = N[0];		m[2 + 4 * 1] = N[1];	m[2 + 4 * 2] = N[2];	m[2 + 4 * 3] = -(eyeX*N[0] + eyeY*N[1] + eyeZ*N[2]);
	m[3 + 4 * 0] = 0;			m[3 + 4 * 1] = 0;		m[3 + 4 * 2] = 0;		m[3 + 4 * 3] = 1; 

	glMultMatrixd(m);
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
	glColor3d(0.0, 0.9, 0.3);
	glutSolidTeapot(TEAPOT_SIZE);
}

void main_keyboard (unsigned char key, int x, int y)
{ /*will use in future assignments for pitch, yaw, roll*/
   switch (key) {
	  case 'P':
		  if(angleP < 360)
			angleP += 5.0;
		  else angleP = 0.0; 

		  glutPostRedisplay();
		  break;
	  case 'p':
		  if (angleP > 0)
			  angleP -= 5.0;
		  else angleP = 360.0;
		  glutPostRedisplay();
		  break;
	  case 'R':
		  if (angleR < 360)
			  angleR += 5.0;
		  else angleR = 0.0;
		  glutPostRedisplay();
		  break;
	  case 'r':
		  if (angleR > 0)
			  angleR -= 5.0;
		  else angleR = 360.0;
		  glutPostRedisplay();
		  break;
	  case 'Y':
		  if (angleY < 360)
			  angleY += 5.0;
		  else angleY = 0.0;
		  glutPostRedisplay();
		  break;
	  case 'y':
		  if (angleY > 0)
			  angleY -= 5.0;
		  else angleY = 360.0;
		  glutPostRedisplay();
		  break;
	  case 'S':
		  if (eyePosition[2] > 0.4) {
			  eyePosition[2] = eyePosition[2] - 0.05;
		  }
		  else { eyePosition[2] = 1.2; }
		  glutPostRedisplay();
		  break;
	  case 's':
		  if (eyePosition[2] < 1.2) {
			  eyePosition[2] = eyePosition[2] + 0.05;
		  }
		  else { eyePosition[2] = 0.4; }
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
	glutSetWindow(screen1);
	glutPositionWindow(GAP, GAP);
	glutReshapeWindow(width-GAP*2, height-GAP*2);
}

void main_display(void)
{
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//String draw functions retrieved from example code by Prof. Lucy Yang
	//(Thank you!)
	glColor3d(0.0, 0.3, 0.7); 
	drawstr(GAP, GAP - 5, "Displaying Teapot");
	glColor3d(0.0, 0.0, 0.0);
	glutSwapBuffers();
}

void screen_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();	
	//gluLookAt(3,-3,3,-3,3,-3,3,-3,3);
	//if (glutGetWindow() == screen1) {
		//Misc Rotation and Translation of image goes here
	//}
	glTranslatef(-5.0, 0.0, 0.0);

	drawTeapot();
	glPopMatrix();
	glutSwapBuffers();
	//glClearColor(0.2, 0.2, 0.2, 0.0);
}


/*

THIS is where the "myPerspective(...)" and "myLookat(...)" functions are called!

*/
void screen_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(0.0, 0.3, 0.7);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90,((screen_width-2*GAP)/(screen_height-2*GAP)),0.1,8);

	myPerspective(90, ((screen_width - 2 * GAP) / (screen_height - 2 * GAP)), 0.1, 8);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

	myLookat(eyePosition[0], eyePosition[1], eyePosition[2], 0, 0, 0, 0, 1, 0);

	glPushMatrix();	// 0, 0, 0, 0, 0, -1, 0, 1, 0
	drawTeapot();
	glPopMatrix();
	glutSwapBuffers();
}

void redisplay_all(void)
{
	glutSetWindow(screen1);
	screen_reshape(screen_width-2*GAP, screen_height-2*GAP);
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
   
   screen1 = glutCreateSubWindow(window, GAP, GAP, screen_width - GAP * 2, screen_height - GAP * 2);
   glutReshapeFunc(screen_reshape);
   glutDisplayFunc(screen_display);
   glutKeyboardFunc(main_keyboard);

   redisplay_all();
   
   glutMainLoop();
   return 0;
}
