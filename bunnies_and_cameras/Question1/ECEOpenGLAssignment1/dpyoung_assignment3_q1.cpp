#include<GL/glut.h>
#include<iostream>
using namespace std;

/*

Name: Daniel Young
Course: ECE595 Computer Graphics
Assignment: 3
Question: 1

Notes: ****Click in the openGL window to get focus. 
	Then, press spacebar repeatedly to display animation.

*/


GLuint window/*, screen2, screen3, screen4, screen5, screen6*/;
static int screen_width = 560/*500*/, screen_height = 480/*500*/;
/*Convert display units to 'stretch' with the window, rather than being tiny pixels...*/
//float xRatio = (float)screen_width / 14;
//float yRatio = (float)screen_height / 12;

static int spaces = 1;

void draw_pixel(int x, int y)
{
	glColor3f(0.0f, 0.9f, 0.3f);
	glPointSize(35.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void swap(float &a, float &b)
{
	float temp = a;
	a = b;
	b = temp;
}

void edgedetect(float x1, float y1, float x2, float y2, int *le, int *re)
{
	float x, mx;
	int i;

	if (y1>y2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	if (y1 == y2)
		mx = x2 - x1;
	else
		mx = (x2 - x1) / (y2 - y1);

	x = x1;
	/*travel along line, follow slope, find edges*/
	for (i = int(y1); i <= (int)y2; i++)
	{
		if (x < (float)le[i]) {
			le[i] = (int)x;
		}
		if (x > (float)re[i]) {
			re[i] = (int)x;
		}
		x += mx;
	}
}

void myScanfill(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5)
{
		//plain arrays, set to the width of the window; changing window width will not change where polygons are filled
		int leftEdge[50], rightEdge[50], i, j;

		for (i = 0; i<50; i++)
			leftEdge[i] = 50, rightEdge[i] = 0;

		edgedetect(x1, y1, x2, y2, leftEdge, rightEdge);
		edgedetect(x2, y2, x3, y3, leftEdge, rightEdge);
		edgedetect(x3, y3, x4, y4, leftEdge, rightEdge);
		edgedetect(x4, y4, x5, y5, leftEdge, rightEdge);
		edgedetect(x5, y5, x1, y1, leftEdge, rightEdge);

		for (j = 0; j < spaces/*50*/; j++)
		{
			if (leftEdge[j] <= rightEdge[j])
				for (i = leftEdge[j]; i<rightEdge[j]; i++)
					draw_pixel(i, j);
		}
}

// 14 12

void display()
{
	float x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;
	x1 = 1.0; y1 = 1.0; x2 = 3.0; y2 = 10.0; x3 = 7.0;
	y3 = 10.0; x4 = 8.0; y4 = 4.0; x5 = 13.0; y5 = 2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);
	myScanfill(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);

	glutSwapBuffers();
}

void myKeyHandler(unsigned char key, int x, int y) 
{
	switch (key) {
	case ' ':
		if (true)
		{
			spaces++;
			if (spaces > 13)
				spaces = 1;
			cout << "Pixel should draw...\n";
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 15.0, 0.0, 13.0);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_width, screen_height);
	window = glutCreateWindow("Assignment 3 Question 1 - Scanline: Press Spacebar Repeatedly");
	glutInitWindowPosition(50, 50);
	glutDisplayFunc(display);
	glutKeyboardFunc(myKeyHandler);
	init();
	glutMainLoop();
}