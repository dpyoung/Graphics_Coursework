#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

static int verticesTotal = 0, facesTotal = 0;
static int screen_width = 500, screen_height = 500;

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

static vector<xyzVertex> myVertexList;
static vector<triangles> myTriangleList;

void init(void) 
{
//	glViewport(0,0,screen_width,screen_height);//Setup for future reference
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0,4.0,-4.0,4.0,-4.0,4.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glPushMatrix();
	/*glShadeModel (GL_FLAT);*/
}

void drawTeapot() {
	glBegin(GL_TRIANGLES);
	for (int i = 1; i < verticesTotal; i++) {
		glVertex3f(myVertexList.at(i).x, myVertexList.at(i).y, myVertexList.at(i).z);
	}
	glEnd();
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   //glPolygonMode(GL_BACK, GL_LINE);

   glMatrixMode(GL_MODELVIEW);
  // glPopMatrix();//pop the camera's current matrix; unused in this assignment, but for future reference
  // glPushMatrix();//push the camera's matrix as the model's... nothing really happens here for now
 //  glRotatef(180, 0.0, 0.0, 1.0);
   glPushMatrix();

   //Cool-color teapots, first quadrant
   for (int i = 1; i <= 3; i++) {
	   glPushMatrix();
	   glRotatef(15.0f*i, 0.0f, 0.0f, 1.0f);
	   glColor3f(0.1f*i,1.0f/i,1.0f);
	   glTranslatef(1.3f*i, -1.0f, 0.f);
	   drawTeapot();
	   glPopMatrix();
   }

   //Warm-color teapots, third quadrant
   for (int i = 1; i <= 3; i++) {
	   glPushMatrix();
	   glColor3f(1.0f*i, 0.1f*i, 1.0f/i);
	   glRotatef(15.0f*i, 0.f, 0.f, 1.f);
	   glTranslatef(-1.3f*i, 1.0f, 0.f);
	   glRotatef(30.f*i, 1.f, 0.f, 0.f);
	   drawTeapot();
	   glPopMatrix();
   }

   //miscellaneous color teapots, vertical wavy line
   for (int i = 1; i <= 10; i++) {
	   glPushMatrix();
	   glColor3f(1.0f/sin(i*0.20f), 0.8f/cos(i*(0.20f)), 0.7f*sin(i*0.20f));
	   glTranslatef(sin(0.5f*i), 5.0f-i, 0.f);
	   glRotatef(20.0f*i, 0.f, 0.3f, 1.f);
	   drawTeapot();
	   glPopMatrix();
   }
   glPopMatrix();
   //glDrawArrays(GL_TRIANGLES, 0, 36); //Maybe I should've used this??
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   /*glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);*/
}

void keyboard (unsigned char key, int x, int y)
{ /*will use in future assignments for pitch, yaw, roll*/
   switch (key) {
      case 'w':   /*  s key rotates at shoulder  */
         //shoulder = (shoulder + 5) % 360;
         //glutPostRedisplay();
         break;
      case 'a':
         //shoulder = (shoulder - 5) % 360;
         //glutPostRedisplay();
         break;
      case 's':  /*  e key rotates at elbow  */
         //elbow = (elbow + 5) % 360;
         //glutPostRedisplay();
         break;
      case 'd':
         //elbow = (elbow - 5) % 360;
         //glutPostRedisplay();
         break;
      default:
         break;
   }
   /*glPopMatrix();
   glLoadIdentity();
   glTranslatef(currentCameraX,currentCameraY,0.f);
   glPushMatrix();*/
}

int loadFileDate() 
{
	string stringLine;
	string delimiter = " ";
	size_t posi = 0;
	string token;
	/*THIS IS WHERE THE MODEL INPUT FILE GOES*/
	ifstream inFile("teapot.poly");
	/*DO NOT EDIT PAST THIS LINE, UNLESS NEW DATA IS AVAILABLE*/
	if (inFile.is_open()) 
	{
		while (!inFile.eof()) 
		{
			float spaceF[3]{};
			int spaceI[3]{};
			string token;
			getline(inFile, stringLine);
			posi = stringLine.find(delimiter);
			stringstream ss(stringLine);
			stringstream(stringLine.substr(0,posi)) >> token;
			if (token == "vertices:") {
				ss >> token >> verticesTotal;
				cout << "Vertices: " << verticesTotal << "\n";
			}
			else if (token == "faces:") {
				ss >> token >> facesTotal;
				cout << "Faces: " << facesTotal << "\n";
			}
			else if (token=="v")
			{
				//cout << "We found a vector... \n";
				ss >> token; //toss out first token
				for (int i = 0; i<3; i++)
				{
					ss >> spaceF[i];
				}
				myVertexList.push_back(xyzVertex
				{
					(GLfloat)spaceF[0],
					(GLfloat)spaceF[1],
					(GLfloat)spaceF[2]
				});
			}
			else if (token=="f") {
				//cout << "We found a face... \n";
				ss >> token; //toss out first token
				for (int i = 0; i<3; i++)
				{
					ss >> spaceI[i];
				}
				myTriangleList.push_back(triangles
				{
					(GLuint)spaceI[0],
					(GLuint)spaceI[1],
					(GLuint)spaceI[2]
				});
			}
			else {
				token = " ignored.";
			}
		}
		cout << endl;
		inFile.close();
	}
	else cout << "failed opening file";
	return 0;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); //GLU_DOUBLE used when movement's involved.
   glutInitWindowSize (screen_width, screen_height); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   loadFileDate(); //begin loading data
   init(); //begin gl stuffs
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
