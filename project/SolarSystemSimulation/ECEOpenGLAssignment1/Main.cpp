#include <iostream>
#include <GL/freeglut.h>  
#include <math.h>
#include <stdarg.h> //for variable moon list
#include <string>
#include <vector>
//#define GAP 0
/**
	Name: Daniel Young
	Course: ECE 595 - Computer Graphics
	Date: November 3rd through November 22nd, 2015
	Course Project

	Main.cpp contains ALL my classes. Yes, I recognize the poor
	design choice that this represents.
		This was done in order to simplify export of my program 
		from Visual Studio. VS is the most horrendous of IDEs.
*/

using namespace std;

const float TO_RADS = 3.141592654f / 180.0f; // The value of 1 degree in radians

GLuint earthTexture;
GLUquadric *earthQuad;	// used for texture mapping; currently non-functional

GLuint window, screen1;

GLint windowWidth = 800;                    // Width of our window
GLint windowHeight = 600;                    // Heightof our window

GLint midWindowX = windowWidth / 2;         // Middle of the window horizontally
GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

GLfloat fieldOfView = 75.0f;
GLfloat nearish = 1.0f;		// The near point of our viewing frustrum
GLfloat farish = 3500.0f;	// The far point of our viewing frustrum (should be far enough to view multiple planets)

GLfloat camXRot = 120.0f;		// Camera rotation
GLfloat camYRot = 0.0f;
GLfloat camZRot = 0.0f;

// Camera position
GLfloat camXPos = 0.0f;
GLfloat camYPos = 1000.0f;
GLfloat camZPos = 0.0f;

// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;

bool running = true;
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12;//GLUT_BITMAP_TIMES_ROMAN_24;


// Location of the sun; irrelevent unless I want to play with it later
GLfloat sunZLocation = 0.0f;// -100.0f;
int sphereHPieces = 150;
int sphereWPieces = 150;
GLint baseSpeed = 1;
GLfloat earthOrbitSpeed = 0.001f; // ==  1/365 year

// Utility Functions

/*****************************************************************************************/
//// src https://www.opengl.org/discussion_boards/archive/index.php/t-169212.html
//int loadBitmap(char *filename)
//{
//	FILE * file;
//	char temp;
//	long i;
//	unsigned int err;
//	BITMAPINFOHEADER infoheader;
//	unsigned char *infoheader_data;
//	GLuint num_texture;
//	if ((err = fopen_s(&file, filename, "rb")) == NULL) return (-1); // Open the file for reading
//	fseek(file, 18, SEEK_CUR);  //start reading width & height 
//	fread(&infoheader.biWidth, sizeof(int), 1, file);
//	fread(&infoheader.biHeight, sizeof(int), 1, file);
//	fread(&infoheader.biPlanes, sizeof(short int), 1, file);
//	if (infoheader.biPlanes != 1) {
//		printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
//		return 0;
//	}
//	// read the bpp
//	fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
//	if (infoheader.biBitCount != 24) {
//		printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
//		return 0;
//	}
//	fseek(file, 24, SEEK_CUR);
//	// read the data
//	if (infoheader.biWidth<0) {
//		infoheader.biWidth = -infoheader.biWidth;
//	}
//	if (infoheader.biHeight<0) {
//		infoheader.biHeight = -infoheader.biHeight;
//	}
//	infoheader_data = (unsigned char *)malloc(infoheader.biWidth * infoheader.biHeight * 3);
//	if (infoheader_data == NULL) {
//		printf("Error allocating memory for color-corrected image data\n");
//		return 0;
//	}
//	if ((i = fread(infoheader_data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
//		printf("Error reading image data from %s.\n", filename);
//		return 0;
//	}
//	for (i = 0; i<(infoheader.biWidth * infoheader.biHeight * 3); i += 3) { // reverse all of the colors. (bgr -> rgb)
//		temp = infoheader_data[i];
//		infoheader_data[i] = infoheader_data[i + 2];
//		infoheader_data[i + 2] = temp;
//	}
//	fclose(file); // Closes the file stream
//	glGenTextures(1, &num_texture);
//	glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter
//											   // The next commands sets the texture parameters
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	// Finally we define the 2d texture
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);
//	// And create 2d mipmaps for the minifying function
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);
//	free(infoheader_data); // Free the memory we used to load the texture
//	return (num_texture); // Returns the current texture OpenGL ID
//}
/*****************************************************************************************/

// Function to convert degrees to radians
float degreesToRadians(const float &theAngleInDegrees)
{
	return theAngleInDegrees * TO_RADS;
}

//draw string utility function
void drawstr(GLuint x, GLuint y, char* format, ...)
{ 	
	va_list args;
	char buffer[255], *s;
	va_start(args, format);
	vsprintf_s(buffer, 40, format, args);
	va_end(args);
	glRasterPos2i(x, y);
	for (s = buffer; *s; s++)
		glutBitmapCharacter(font_style, *s);
}

//Celestial Body Classes
class Moon
{
public: //because I'm lazy and don't have time for getters or setters
	GLubyte planetColor[3];
	GLfloat orbitDistance;
	GLfloat orbitSpeed;
	GLfloat orbitRotation;
	double planetSize;

	Moon() {
		planetColor[0] = 0;
		planetColor[1] = 0;
		planetColor[2] = 0;
		orbitDistance = 0;
		orbitSpeed = 0;
		orbitRotation = 0;
		planetSize = 1;
	}

	Moon(int a, int b, int c, GLfloat d, GLfloat s, GLfloat r, double size) {
		planetColor[0] = a;
		planetColor[1] = b;
		planetColor[2] = c;
		orbitDistance = d;
		orbitSpeed = s;
		orbitRotation = r;
		planetSize = size;
	};
};

class Planet
{
public: //just because I'm lazy
	GLubyte planetColor[3];
	GLfloat orbitDistance;
	GLfloat orbitSpeed;
	GLfloat orbitRotation;
	double planetSize;
	GLfloat planetLocation[3];
	char* planetName;
	
public: Planet(int a, int b, int c, GLfloat d, GLfloat s, GLfloat r, double size, char* name) {
		planetColor[0] = a;
		planetColor[1] = b;
		planetColor[2] = c;
		orbitDistance = d;
		orbitSpeed = s;
		orbitRotation = r;
		planetSize = size;
		planetLocation[0] = d*cos(r);
		planetLocation[1] = 0;
		planetLocation[2] = d*sin(r);
		planetName = name;
	};

	void updatePosition();
	string nameOfPlanet();
	void drawName();
};

string Planet::nameOfPlanet()
{
	return string(planetName);
}

void Planet::updatePosition() {
	planetLocation[0] = orbitDistance*cos(orbitRotation);
	planetLocation[2] = orbitDistance*sin(orbitRotation);
};

void Planet::drawName() {
	drawstr(50, 50, planetName);
}

//Declaration and initialization of Celestial Bodies
Planet earth = { 0, 0, 255, (GLfloat) 2.5f*150.0f, (GLfloat)earthOrbitSpeed, (GLfloat) 0.0f, 0.64, "Earth"};
Moon luna = { 120, 120, 120, (GLfloat) 3.84f, (GLfloat) earthOrbitSpeed*13.5f, (GLfloat) 0.0f, 0.1737 };
Planet mercury = { 90, 80, 120, (GLfloat)2.5f*58.0f, (GLfloat) earthOrbitSpeed*365.0f/88.0f, (GLfloat)0.0f, 0.244, "Mercury" };
Planet venus = {130, 80, 0, (GLfloat) 2.5f*108.0f, (GLfloat) earthOrbitSpeed*365.0f/225.0f, (GLfloat)0.0f, 0.605, "Venus" };
Planet mars = {150, 0, 0, (GLfloat) 2.5f*228.0f, (GLfloat) earthOrbitSpeed*365.0f/687.0f, (GLfloat) 0.0f, 0.339, "Mars" };
Moon phobos = { 120, 90, 120, (GLfloat)2.8f, (GLfloat) earthOrbitSpeed*365.0f*1.263f, (GLfloat)0.0f, 0.03 };
Moon deimos = { 150, 140, 125, (GLfloat)6.8f, (GLfloat) earthOrbitSpeed*365.0f/3.3f, (GLfloat)0.0f, 0.1 };
Planet jupiter = { 170, 80, 30, (GLfloat)2.5f * 778.0f, (GLfloat)earthOrbitSpeed / 12.0f, (GLfloat) 0.0f, 6.99, "Jupiter" };
Moon io = { 120, 140, 80, (GLfloat)4.2f*2.0f, (GLfloat)earthOrbitSpeed*365.0f/1.77f, (GLfloat)0.0f, 0.36 };
Moon europa = { 160, 160, 200, (GLfloat)6.7f*2.0f, (GLfloat)earthOrbitSpeed*365.0f/3.55f, (GLfloat)0.0f, 0.31 };
Moon ganymede = { 130, 150, 125, (GLfloat)10.7f*2.0f, (GLfloat)earthOrbitSpeed*365.0f/7.15f, (GLfloat)0.0f, 0.52 };
Moon callisto = { 70, 50, 120, (GLfloat)18.8f*2.0f, (GLfloat)earthOrbitSpeed*365.0f/16.69f, (GLfloat)0.0f, 0.48 };

static vector<Planet> planetsInSolarSystem;

//NOTES FROM SOURCE:
// Set the light source location to be the same as the sun position
// Don't forget that the position is a FOUR COMPONENT VECTOR (with the last component as w) if you omit the last component expect to get NO LIGHT!!!
GLfloat  lightPos[] = { 0.0f, 0.0f, -300.0f, 1.0f };
GLfloat moveSpeedFactor = 6.0f;
double inProximity = 280.0;

// Hoding any keys down?
bool holdingForward = false;
bool holdingBackward = false;
bool holdingLeftStrafe = false;
bool holdingRightStrafe = false;

//detect distance from camera; Bugs exist
bool proximityDetect(Planet* planet) {
	double xDif, yDif, zDif;

	if (camXPos > planet->planetLocation[0])
		xDif = (double)(camXPos - planet->planetLocation[0]);
	else
		xDif = (double)(planet->planetLocation[0] - camXPos);
	if (camYPos > planet->planetLocation[1])
		yDif = (double)(camYPos - planet->planetLocation[1]);
	else
		yDif = (double)(planet->planetLocation[1] - camYPos);
	if (camZPos > planet->planetLocation[2])
		zDif = (double)(camZPos - planet->planetLocation[2]);
	else
		zDif = (double)(planet->planetLocation[2] - camZPos);
	double xDifS = xDif*xDif;
	double yDifS = yDif*yDif;
	double zDifS = zDif*zDif;
	//inProximity is squared value; so, sum of three square values is equivalent in check
	if (zDifS + xDifS + yDifS < inProximity){
	//zDif = (double)(camZPos - planet->planetLocation[2]);
	//if (xDif*xDif + zDif*zDif < inProximity) {
		return true;
	}
	return false;
}

//Initialize the oodles of GL vars
void init()
{
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	glutSetCursor(GLUT_CURSOR_NONE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fieldOfView/1.0f, windowWidth/windowHeight , nearish, farish);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set out clear color to black, full alpha
										  //	glfwSwapInterval(1);        // Lock to vertical sync of monitor (normally 60Hz, so 60fps)
	glShadeModel(GL_SMOOTH);    // Enable (gouraud) shading
	glEnable(GL_DEPTH_TEST);    // Enable depth testing
	glClearDepth(1.0f);         // Clear the entire depth of the depth buffer
	glDepthFunc(GL_LEQUAL);		// Set our depth function to overwrite if new value less than or equal to current value
	glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Ambient, diffuse and specular lighting values
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLint specularMagnitude = 64; // Define how "tight" our specular highlights are (larger number = smaller specular highlight). The valid range is is 1 to 128

	// Setup and enable a light 
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);          // Specify the position of the light
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);      // Specify ambient light properties
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);      // Specify diffuse light properties
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);     // Specify specular light properties
	glEnable(GL_LIGHT0);
	// Enable color tracking of materials
	glEnable(GL_COLOR_MATERIAL);

	// Define the shininess of the material used in drawing; MAXIMUM SHINY!
	GLfloat materialSpecularReflectance[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Use our shiny material and magnitude
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecularReflectance);
	glMateriali(GL_FRONT, GL_SHININESS, specularMagnitude);
//	earthQuad = gluNewQuadric();
//	gluQuadricTexture( earthQuad, GL_TRUE);
//	earthTexture = loadBitmap("earth_texture.bmp");
}

// Function to move the camera the amount we've calculated in the calculateCameraMovement function
void moveCamera()
{
	camXPos += camXSpeed;
	camYPos += camYSpeed;
	camZPos += camZSpeed;
}

// Function to deal with mouse position changes, called whenever the mouse cursor moves
void handleMouseMove(/*GLint button, GLint action,*/ GLint mouseX, GLint mouseY)
{
	GLfloat vertMouseSensitivity = 10.0f;
	GLfloat horizMouseSensitivity = 10.0f;

	int horizMovement = mouseX - midWindowX;
	int vertMovement = mouseY - midWindowY;

	camXRot += vertMovement / vertMouseSensitivity;
	camYRot += horizMovement / horizMouseSensitivity;
	// Limit loking up to straight-vertical
	if (camXRot < -90.0f)
	{
		camXRot = -90.0f;
	}
	// Limit looking down to straight-vertical
	if (camXRot > 90.0f)
	{
		camXRot = 90.0f;
	}
	// Looking left and right.
	if (camYRot < -180.0f)
	{
		camYRot += 360.0f;
	}
	if (camYRot > 180.0f)
	{
		camYRot -= 360.0f;
	}
	// Reset the mouse position to the centre of the window each frame
	if (mouseX != midWindowX || mouseY != midWindowY) glutWarpPointer(midWindowX, midWindowY); //glutWarpPointer(midWindowX, midWindowY);
}

// Function to calculate which direction we need to move the camera and by what amount
void calculateCameraMovement()
{
	float camMovementX = 0.0f;
	float camMovementY = 0.0f;
	float camMovementZ = 0.0f;

	if (holdingForward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(degreesToRadians(camXRot));
		camMovementX += (moveSpeedFactor * float(sin(degreesToRadians(camYRot)))) * pitchFactor;
		// Control Y-Axis movement
		camMovementY += moveSpeedFactor * float(sin(degreesToRadians(camXRot))) * -1.0f;
		// Control Z-Axis movement
		float yawFactor = float(cos(degreesToRadians(camXRot)));
		camMovementZ += (moveSpeedFactor * float(cos(degreesToRadians(camYRot))) * -1.0f) * yawFactor;
	}

	if (holdingBackward == true)
	{
		// Control X-Axis movement
		float pitchFactor = cos(degreesToRadians(camXRot)); 
		camMovementX += (moveSpeedFactor * float(sin(degreesToRadians(camYRot))) * -1.0f) * pitchFactor;
		// Control Y-Axis movement
		camMovementY += moveSpeedFactor * float(sin(degreesToRadians(camXRot)));
		// Control Z-Axis movement
		float yawFactor = float(cos(degreesToRadians(camXRot)));
		camMovementZ += (moveSpeedFactor * float(cos(degreesToRadians(camYRot)))) * yawFactor;
	}

	if (holdingLeftStrafe == true)
	{
		float yRotRad = degreesToRadians(camYRot);
		camMovementX += -moveSpeedFactor * float(cos(yRotRad));
		camMovementZ += -moveSpeedFactor * float(sin(yRotRad));
	}

	if (holdingRightStrafe == true)
	{
		float yRotRad = degreesToRadians(camYRot);
		camMovementX += moveSpeedFactor * float(cos(yRotRad));
		camMovementZ += moveSpeedFactor * float(sin(yRotRad));
	}
	// combining movements for all keys pressed, assign them to camera speed along the given axiis
	camXSpeed = camMovementX;
	camYSpeed = camMovementY;
	camZSpeed = camMovementZ;

	// X Speed cap
	if (camXSpeed > moveSpeedFactor)
	{
		camXSpeed = moveSpeedFactor;
	}
	if (camXSpeed < -moveSpeedFactor)
	{
		camXSpeed = -moveSpeedFactor;
	}
	// Y Speed cap
	if (camYSpeed > moveSpeedFactor)
	{
		camYSpeed = moveSpeedFactor;
	}
	if (camYSpeed < -moveSpeedFactor)
	{
		camYSpeed = -moveSpeedFactor;
	}
	// Z Speed cap
	if (camZSpeed > moveSpeedFactor)
	{
		camZSpeed = moveSpeedFactor;
	}
	if (camZSpeed < -moveSpeedFactor)
	{
		camZSpeed = -moveSpeedFactor;
	}
	//reset our movements; equal to releasing keys
	holdingForward = false;
	holdingBackward = false;
	holdingLeftStrafe = false;
	holdingRightStrafe = false;
}

// for varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
void renderPlanet(Planet* planet, int numberOfMoons, ...) {
	glColor3ub(120, 120, 120);
	glPointSize(2.0f);
	glBegin(GL_POINTS);                      // Select points as the primitive
	for (int i = 0; i < 360; i++) {
		glVertex3f(planet->orbitDistance*cos(TO_RADS*i), 0.0f, planet->orbitDistance*sin(TO_RADS*i));
	}
	glEnd();                                 // Done drawing points
	glPushMatrix();
	glColor3ub(planet->planetColor[0], planet->planetColor[1], planet->planetColor[2]);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glRotatef(planet->orbitRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(planet->orbitDistance, 0.0f, 0.0f);
	glutSolidSphere(planet->planetSize, sphereHPieces, sphereWPieces);
	va_list ap;
	va_start(ap, numberOfMoons);
	for (int i = 3; i <= numberOfMoons+2; i++) {
		//Preserve position of planet before rendering position of moon
		glPushMatrix();
		Moon * current = va_arg(ap, Moon*);
		glRotatef((current->orbitRotation), 0.0f, 1.0f, 0.0f);
		glTranslatef((current->orbitDistance), 0.0f, 0.0f);
		//Moon Rotation
		(current->orbitRotation) += (current->orbitSpeed) * baseSpeed;
		if ((current->orbitRotation) > 360.0f)
		{
			(current->orbitRotation) -= 360.0f;
		};
		//moon
		/*RENDER MOON HERE*/
		glColor3ub(current->planetColor[0], current->planetColor[1], current->planetColor[2]);
		glutSolidSphere((current->planetSize), sphereHPieces, sphereWPieces);
		glPopMatrix();
	}
	va_end(ap);
	//Finish with planet rotation
	(planet->orbitRotation) += (planet->orbitSpeed) * baseSpeed;
	if ((planet->orbitRotation) > 360.0f)
	{
		(planet->orbitRotation) -= 360.0f;
	}
	planet->updatePosition();
	glPopMatrix();
}


void renderPlanet(Planet* planet) {
	glColor3ub(120, 120, 120);
	glPointSize(2.0f);
	glBegin(GL_POINTS);                      // Select points as the primitive
	for (int i = 0; i < 360; i++) {
		glVertex3f(planet->orbitDistance*cos(TO_RADS*i), 0.0f, planet->orbitDistance*sin(TO_RADS*i));
	}
	glEnd();
	//glEnable(GL_TEXTURE_2D);///////////////
	//glBindTexture(GL_TEXTURE_2D, earthTexture);//////////////
	glPushMatrix();
	glColor3ub(planet->planetColor[0], planet->planetColor[1], planet->planetColor[2]);
	glRotatef(planet->orbitRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(planet->orbitDistance, 0.0f, 0.0f);
	glutSolidSphere(planet->planetSize, sphereHPieces, sphereWPieces);
	//gluSphere(earthQuad, planet->planetSize, sphereHPieces, sphereWPieces);
	//planet rotation
	(planet->orbitRotation) += (planet->orbitSpeed) * baseSpeed;
	if ((planet->orbitRotation) > 360.0f)
	{
		(planet->orbitRotation) -= 360.0f;
	}
	planet->updatePosition();
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);////////////////
}

// Function to set flags according to which keys are pressed or released
void main_keyboard(unsigned char theKey, int x, int y)
{
	switch (theKey)
	{
	case 'W': //accelerate
		//holdingForward = true;
		moveSpeedFactor += 0.5;
		if (moveSpeedFactor > 9000) {//It's over 9000!
			moveSpeedFactor = 9000;
		}
		break;
	case 'w':
		holdingForward = true;
		break;
	case 'S': //decelerate
		//holdingBackward = true;
		moveSpeedFactor -= 0.5;
		if (moveSpeedFactor < 1) {//keep movement possible
			moveSpeedFactor = 1;
		}
		break;
	case 's':
		holdingBackward = true;
		break;
	case 'a':
		holdingLeftStrafe = true;
		break;
	case 'd':
		holdingRightStrafe = true;
		break;
	case ' ':
		glutExit();
		break;
	case '-': //multiple presses "pauses" system
		if (baseSpeed > 0)
			baseSpeed--;
		else baseSpeed = 0;
		break;
	case '+':
		if (baseSpeed < 40)
			baseSpeed++;
		else { baseSpeed = 40; }
		break;
	default:
		// Do nothing...
		break;
	}
}

// Function to draw our spheres and position the light source
void main_display()
{
	calculateCameraMovement();
	moveCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Move the camera to our location in space
	glRotatef(camXRot, 1.0f, 0.0f, 0.0f);        // Rotate our camera on the x-axis (looking up and down)
	glRotatef(camYRot, 0.0f, 1.0f, 0.0f);        // Rotate our camera on the  y-axis (looking left and right)
	glTranslatef(-camXPos, -camYPos, -camZPos);    // Translate the model view matrix to the position of our camera
	//Not really dimensional; the sun is just a big dot

	GLfloat newLightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, newLightPos);  // Place the light where the sun is!

	glTranslatef(0.0f, 0.0f, sunZLocation);
	int sunColorDepth1 = rand() % 35;
	int sunColorDepth2 = rand() % 55;
	glColor3ub(200+sunColorDepth2, 220+sunColorDepth1, 0);

	glDisable(GL_LIGHTING); //allows sun to be super-bright and plain-color flatish
	glutSolidSphere(69.5f, sphereHPieces, sphereWPieces);
	glEnable(GL_LIGHTING);
	//3D rendering Time
	renderPlanet(&earth, 1, &luna);
	renderPlanet(&venus);
	renderPlanet(&mercury);
	renderPlanet(&mars, 2, &deimos, &phobos);
	renderPlanet(&jupiter, 4, &io, &europa, &ganymede, &callisto);
	//HUD
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColor3f(0.9, 1.0, 0.9);
	//DRAW PLANET NAME OF CHOICE HERE
	Planet *i;
	bool nearAPlanet = false;
	for (i = &planetsInSolarSystem[0]; i != &planetsInSolarSystem[0] + 5; ++i) {
		if (proximityDetect(i))
		{
			(*i).drawName();
			nearAPlanet = true;
		}
	}
	if (nearAPlanet == false) {
		drawstr(20, 20, "Wandering...");
	}
	string tempX = to_string(camXPos);
	char* tempXArg = (char*) tempX.c_str();
	drawstr(20, windowHeight-20, tempXArg);
	drawstr(100, windowHeight - 20, "X Pos");

	string tempY = to_string(camYPos);
	char* tempYArg = (char*)tempY.c_str();
	drawstr(20, windowHeight - 34, tempYArg);
	drawstr(100, windowHeight - 34, "Y Pos");

	string tempZ = to_string(camZPos);
	char* tempZArg = (char*)tempZ.c_str();
	drawstr(20, windowHeight - 48, tempZArg);
	drawstr(100, windowHeight - 48, "Z Pos");
	//END DRAW
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}

// Fire it up...
int main(int argc, char **argv)
{
	//GLuint earthTexture = loadBitmap("earth_texture.bmp");
	planetsInSolarSystem.push_back(earth);
	planetsInSolarSystem.push_back(venus);
	planetsInSolarSystem.push_back(mercury);
	planetsInSolarSystem.push_back(mars);
	planetsInSolarSystem.push_back(jupiter);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(50, 50);
	cout << "Controls: Use WSAD and the mouse to move around!" << endl;
	cout << "          The camera direction is controlled by the mouse." << endl;
	cout << "NOTE:     Mercury starts at X = 145, Y = 0, Z = 0." << endl;
	// ----- Intialiase FreeGLUT -----
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// Create a window
	window = glutCreateWindow("Model Solar System");
	init();
	glutDisplayFunc(main_display);
	glutKeyboardFunc(main_keyboard);
	glutPassiveMotionFunc(handleMouseMove);
	glutIdleFunc(main_display);
	glutMainLoop();
	return 0;
}