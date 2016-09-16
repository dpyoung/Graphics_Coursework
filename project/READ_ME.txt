Name: Daniel Young (no group members)
Course: ECE 595 - Computer Graphics
Date: 15 December 2015
Submission: Course Project


Contents
	This zip folder submission contains the Visual C++ source folder
required to run the "Solar System Simulation" program created for this course.
Note that the Visual C++ solution file is included, but if this solution fails
to open for whatever reason, the source file itself is "main.cpp", located at
the folder "SolarSystemSimulation\ECEOpenGLAssignment1". Also within the root
of this zip folder is the project's report, titled 
"Daniel Young - Graphics Report.docx"
	
Running the Simulation
	An .exe file may be found at "SolarSystemSimulation\Debug", but in
the situation that this file does not run, the source may be compiled from
Visual Studio 2013 and 2015 (project build settings may need changed to 
compile in Visual Studio 2013 or older, however). FreeGlut was the only
external library used in the making of this program.

Using the Simulation
	Upon execution of the program, the user is presented with a black
openGL window. Once the window gains focus with the mouse, the sun is focused
on. The user may use the following keys to navigate:
	"w" moves the camera forward.
	"s" moves the camera backward.
	"a" strafes the camera to the left.
	"d" strafes the camera to the right.
	The mouse directs the camera.
	"-" slows the simulation's speed (up to a complete pause).
	"+" speeds the simulation up to 40x speed (note: "shift" key)
	Spacebar closes the simulation.
The user is provided with a Heads-Up Display of the camera's current location
in space. To find a planet at simulation start-up, the user should approach
coordinate values of x = 150, y = 0, z = 0. These coordinates match those of
"Mercury", or at least it's starting point. Note that while the simulation 
runs, the planets rotate around the sun, so these coordinates will change.
	Planetary orbits will disappear as the user approaches for asthetic
reasons. This is not a bug; it was an intentional "happy accident." 


Known Issues
	This simulation currently does not update the HUD properly when the
camera is within close proximity to planets. It will update properly along 
Z and Y values = 0, with a varying X value alone. 