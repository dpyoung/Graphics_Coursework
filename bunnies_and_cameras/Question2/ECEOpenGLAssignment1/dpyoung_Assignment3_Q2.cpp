#include<GL/glut.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h> 
using namespace std;
#define GAP  20 /* gap between subwindows, trick learned from class */
#define PI 3.14159265
#define SPEED_FACTOR 1;

/*
Name: Daniel Young
Course: ECE595 Computer Graphics
Assignment: 3
Question: 2

Notes: 
*/

//static int verticesTotal = 0, facesTotal = 0;
GLuint window;
//GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;
GLdouble projection[16], modelview[16], inverse[16];
static double N[3], U[3], V[3];
static int verticesTotal = 0, facesTotal = 0;
static int screen_width = 500, screen_height = 500;
GLuint theBunny1, theBunny2;
//Cameras***
float xpos = 0, ypos = 0, zpos = 2.0, xrot = 0, yrot = 0, angle = 0.0;
static int clipNumber = 0;
static bool isWireFrame = false;
static double angleP = 0.0, angleY = 0.0, angleR = 0.0;
static double zoomFactor = 1.00;
//GLdouble eyePosition[3] = { 0.0, 0.0, -6.0 };
float lastx, lasty;
//Clippings***
GLint i = 1;
double clip_count = 0;
GLdouble planeEqnNull[] = { 0.0, 0.0, 0.0, 0.0 };\
GLUquadricObj *quadObj;

struct xyzVertex {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct triangles {
	GLuint a;
	GLuint b;
	GLuint c;
};

struct modelObj {
	int numVertices;
	int numFaces;
	vector<xyzVertex> myVertices;
	vector<triangles> myFaces;
};

static vector<xyzVertex> myVertexList;
static vector<triangles> myTriangleList;
static int bunnyMultiples = 1;


/******************************************************************************************************************************************/

void swap(float &a, float &b)
{
	float temp = a;
	a = b;
	b = temp;
}

/******************************************************************************************************************************************/

/*My Rotation function, translated from lecture materials*/
void myRotatef(float degrees, float x, float y, float z) {
	GLdouble  m[16] = { 0.0f };
	/*x rot*/
	m[0 + 4 * 0] = 1;	m[0 + 4 * 1] = 0;						m[0 + 4 * 2] = 0;						m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0;	m[1 + 4 * 1] = cos(x*degrees*PI / 180);	m[1 + 4 * 2] = -sin(x*degrees*PI / 180);	m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0;	m[2 + 4 * 1] = sin(x*degrees*PI / 180);	m[2 + 4 * 2] = cos(x*degrees*PI / 180);	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;	m[3 + 4 * 1] = 0;						m[3 + 4 * 2] = 0;						m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
	/*y rot*/
	m[0 + 4 * 0] = cos(y*degrees*PI / 180);	m[0 + 4 * 1] = 0;	m[0 + 4 * 2] = sin(y*degrees*PI / 180);	m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0;						m[1 + 4 * 1] = 1;	m[1 + 4 * 2] = 0;						m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = -sin(y*degrees*PI / 180);	m[2 + 4 * 1] = 0;	m[2 + 4 * 2] = cos(y*degrees*PI / 180);	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;						m[3 + 4 * 1] = 0;	m[3 + 4 * 2] = 0;						m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
	/*z rot*/
	m[0 + 4 * 0] = cos(z*degrees*PI / 180);	m[0 + 4 * 1] = -sin(z*degrees*PI / 180);	m[0 + 4 * 2] = 0;	m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = sin(z*degrees*PI / 180);	m[1 + 4 * 1] = cos(z*degrees*PI / 180);	m[1 + 4 * 2] = 0;	m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0;						m[2 + 4 * 1] = 0;						m[2 + 4 * 2] = 1;	m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0;						m[3 + 4 * 1] = 0;						m[3 + 4 * 2] = 0;	m[3 + 4 * 3] = 1;
	glMultMatrixd(m);
}

/******************************************************************************************************************************************/

void setProjectionMatrix(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0*zoomFactor, (float)width / (float)height, 0.1, 200);
	/* ...Where 'zNear' and 'zFar' are up to you to fill in. */
}

/******************************************************************************************************************************************/

void drawModel() {
	glBegin(GL_TRIANGLES);
	for (int i = 1; i < facesTotal; i++) {
		glVertex3f(myVertexList.at(myTriangleList.at(i - 1).a - 1).x, myVertexList.at(myTriangleList.at(i - 1).a - 1).y, myVertexList.at(myTriangleList.at(i - 1).a - 1).z);
		glVertex3f(myVertexList.at(myTriangleList.at(i - 1).b - 1).x, myVertexList.at(myTriangleList.at(i - 1).b - 1).y, myVertexList.at(myTriangleList.at(i - 1).b - 1).z);
		glVertex3f(myVertexList.at(myTriangleList.at(i - 1).c - 1).x, myVertexList.at(myTriangleList.at(i - 1).c - 1).y, myVertexList.at(myTriangleList.at(i - 1).c - 1).z);
	}
	glEnd();
}

/******************************************************************************************************************************************/

void camera(void) {
	glRotatef(xrot, 1.0, 0.0, 0.0);  //rotate our camera on teh x - axis(left and right)
	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y - axis(up and down)
	//glRotatef(zrot, 0.0, 0.0, 1.0);
	glTranslated(-xpos, -ypos, -zpos); //translate the screento the position of our camera
}

/******************************************************************************************************************************************/

void display(void)
{
	//	eye offset from center
	float offset = 1.0;
	//	left-hand viewport
	glViewport(0, 0, screen_width, screen_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();
	
	glRotatef(angleP, 1.0f, 0.0f, 0.0f);
	glRotatef(angleY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleR, 0.0f, 0.0f, 1.0f);
	glTranslatef(-xpos, -ypos, -zpos);

//	glPushMatrix();
	gluQuadricDrawStyle(quadObj, GLU_LINE);
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(30, 1.0, 0.0, 0.0);

	//wire frame toggle
	if (isWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//clipping toggle
	if (clipNumber == 1) {
		GLdouble planeEqn[] = { clip_count, -0.2, 0.0, 0.0 };  /* initial clipping plane eqn */
		glClipPlane(GL_CLIP_PLANE0, planeEqn);  /* define clipping plane */
		glEnable(GL_CLIP_PLANE0);  /* and enable it */
	}
	else {
		//glClipPlane(GL_CLIP_PLANE0, planeEqnNull);
		//glEnable(GL_CLIP_PLANE0);
	}

	glCallList(theBunny1);

	if (clipNumber == 1) {
		glDisable(GL_CLIP_PLANE0);
	}

//	glPopMatrix();

	glutSwapBuffers();	
}
/******************************************************************************************************************************************/
void myinit(void)
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	setProjectionMatrix(screen_width, screen_height);

	GLfloat light_position[] = { xpos, ypos, zpos, 1.0 };
	GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor(0.0, 0.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

	glLightModeliv(GL_LIGHT_MODEL_TWO_SIDE, &i); // two-sided lighting attributes
	glEnable(GL_LIGHTING);		// so lighting models are used
	glEnable(GL_LIGHT0);		// we'll use LIGHT0
	glEnable(GL_DEPTH_TEST);	// allow z-buffer display
	glEnable(GL_CLIP_PLANE1);	// enable clipping on plane 1
	glEnable(GL_BLEND);      // enable alpha-channel blending        
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	theBunny1 = glGenLists(1);
	glNewList(theBunny1, GL_COMPILE);
	drawModel();
	glEndList();
	glShadeModel(GL_FLAT);
	glClearColor(0.2, 0.2, 0.0, 0.0);
}

/******************************************************************************************************************************************/

//Utility function for reference from Prof. Lucy Yang's code on projections
void identityD(GLdouble m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

/******************************************************************************************************************************************/

/*My Translation function, translated from lecture materials*/
void myTranslatef(float x, float y, float z) {
	GLfloat  m[16] = { 0.0f };
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = x;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = y;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = z;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
	glMultMatrixf(m);
}

/******************************************************************************************************************************************/

void normalize(double * vect) {
	double length = sqrt(vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2]);
	vect[0] = length*vect[0] / abs(length);
	vect[1] = length*vect[1] / abs(length);
	vect[2] = length*vect[2] / abs(length);
}


/******************************************************************************************************************************************/

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
	V[0] = N[1] * U[2] - N[2] * U[1];
	V[1] = N[2] * U[0] - N[0] * U[2];
	V[2] = N[0] * U[1] - N[1] * U[0];
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

	m[0 + 4 * 0] = U[0];		m[0 + 4 * 1] = U[1];	m[0 + 4 * 2] = U[2];	m[0 + 4 * 3] = -(eyeX*U[0] + eyeY*U[1] + eyeZ*U[2]);
	m[1 + 4 * 0] = V[0];		m[1 + 4 * 1] = V[1];	m[1 + 4 * 2] = V[2];	m[1 + 4 * 3] = -(eyeX*V[0] + eyeY*V[1] + eyeZ*V[2]);
	m[2 + 4 * 0] = N[0];		m[2 + 4 * 1] = N[1];	m[2 + 4 * 2] = N[2];	m[2 + 4 * 3] = -(eyeX*N[0] + eyeY*N[1] + eyeZ*N[2]);
	m[3 + 4 * 0] = 0;			m[3 + 4 * 1] = 0;		m[3 + 4 * 2] = 0;		m[3 + 4 * 3] = 1;

	glMultMatrixd(m);
}


/******************************************************************************************************************************************/

void myKeyHandler(unsigned char key, int x, int y) 
{
	switch (key) {
	case ' ':
		exit(0);
		break;
	case '-':
		{
			zoomFactor += 0.05;
			if (zoomFactor <= 0.05)
				zoomFactor = 0.1;
		}
		break;
	case '+':
		{
			zoomFactor += 0.05;
			if (zoomFactor >= 1.45)
				zoomFactor = 1.40;
		}
		break;
	case 'c':
	{
		if (clipNumber == 1) {
			clipNumber = 0;
		}
		else {
			clipNumber = 1;
		}
		break;
	}
	case 'w':
		{
			float xrotrad, yrotrad, zrotrad;
			yrotrad = (yrot / 180.0 * PI);
			xrotrad = (xrot / 180.0 * PI);
			//zrotrad = (zrot / 180.0 * PI);
			xpos += float(sin(yrotrad))*0.10;
			zpos -= float(cos(yrotrad))*0.10;
			ypos -= float(sin(xrotrad))*0.10;
		}
		break;
	case 'a':
		{
			float yrotrad;
			yrotrad = (yrot / 180.0 * PI);
			xpos -= float(cos(yrotrad)) * 0.2;
			zpos -= float(sin(yrotrad)) * 0.2;
		}
		break;
	case 's':
		{
			float xrotrad, yrotrad, zrotrad;
			yrotrad = (yrot / 180.0 * PI);
			xrotrad = (xrot / 180.0 * PI);
			//zrotrad = (zrot / 180.0 * PI);
			xpos -= float(sin(yrotrad))*0.10;
			zpos += float(cos(yrotrad))*0.10;
			ypos += float(sin(xrotrad))*0.10;
		}
		break;
	case 'd':
		{
			float yrotrad;
			yrotrad = (yrot / 180.0 * PI);
			xpos += float(cos(yrotrad)) * 0.2;
			zpos += float(sin(yrotrad)) * 0.2;
		}
		break;
	case 'q':
		{
			xrot += 1;
			if (xrot > 360) xrot -= 360;

		}
		break;
	case 'e':
		{
			xrot -= 1;
			if (xrot < -360) xrot += 360;
		}
		break;
	case 't':
		{
			if (isWireFrame) {
				isWireFrame = false;
			}
			else
				isWireFrame = true;
		}
		break;
	case 'y':
	{
		if (clip_count < 2.0) {
			clip_count += 0.05;
		}
		else
			clip_count = 2.0;
	}
	break;
	case 'u':
	{
		if (clip_count > -2.0) {
			clip_count -= 0.05;
		}
		else
			clip_count = -2.0;
	}
	break;

	default:
		break;
	}
	glutPostRedisplay();
}

/******************************************************************************************************************************************/

void myMouseHandler(int x, int y) {
	int diffx = x - lastx; //check the difference between the current x and the last x position
	int diffy = y - lasty; //check the difference between the current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position
	xrot += (float)diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float)diffx;// set the xrot to yrot with the addition of the difference in the x position
//	glutWarpPointer(screen_width/2, screen_height/2); //Locks up window until clicked. Sad; I wanted this to work!
}

/******************************************************************************************************************************************/

void redisplay_all(int width, int height)
{
	setProjectionMatrix(width, height);
	glutPostRedisplay();
}

/******************************************************************************************************************************************/
int loadFileDate(string name)
{
	string stringLine;
	string delimiter = " ";
	size_t posi = 0;
	string token;
	/*THIS IS WHERE THE MODEL INPUT FILE GOES*/
	ifstream inFile(name);

	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			getline(inFile, stringLine);
			stringstream ss(stringLine);
			string word;
			ss >> word; 
			if (word.compare("vertices:") == 0) {
				ss >> verticesTotal;
				cout << "vertices: " << verticesTotal << endl;
			}
			else if (word.compare("faces:") == 0) {
				ss >> facesTotal;
				cout << "faces: " << facesTotal << endl;
				cout << "Found vertices and faces, please wait a while... (my parser's slow)" << endl;
				cout << "Even if you think this program's frozen, it's still working." << endl;
				cout << " ... I promise." << endl;
			}
			else if (word.compare("v") == 0) {
				float spaceF[3]{0};
				ss >> spaceF[0] >> spaceF[1] >> spaceF[2];
				myVertexList.push_back(xyzVertex{
					(GLfloat)spaceF[0],
					(GLfloat)spaceF[1],
					(GLfloat)spaceF[2] });	
			}
			else if (word.compare("f") == 0) {
					int spaceI[3]{0};
					ss >> spaceI[0] >> spaceI[1] >> spaceI[2];
					myTriangleList.push_back(triangles{
						(GLuint)spaceI[0],
						(GLuint)spaceI[1],
						(GLuint)spaceI[2] });
			}
			else {
				token = " ignored.";
				cout << token << endl;
			}
		}
		cout << endl;
		inFile.close();
	}
	else cout << "failed opening file";
	return 0;
}
/******************************************************************************************************************************************/



// Main function: you may change the file name passed to the model function here, in "loadFileDate( ... )"
// Please note that I mispelled that function intentionally, and left it misspelled as a momento for myself.
void main(int argc, char **argv)
{
	quadObj = gluNewQuadric();
	loadFileDate("bunny.poly");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_width, screen_height);
	window = glutCreateWindow("Assignment 3 Question 2 - View The Bunny!");
	glutInitWindowPosition(50, 50);
	glutDisplayFunc(display);
	glutReshapeFunc(redisplay_all);
	glutKeyboardFunc(myKeyHandler);
	glutPassiveMotionFunc(myMouseHandler);
	glutIdleFunc(display);
	myinit();
	glutMainLoop();
}