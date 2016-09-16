Name: Daniel Young
Course: ECE 595 - Computer Graphics
Date: November 15, 2015
Assignment: 3


Question 1:
	Source .cpp file = dpyoung_assignment3_q1.cpp
	Visual Studio solution found in folder Question1.
This program simulates the "scanline fill" algorithm for 2-D polygons. To step
through the program, first 
- *click in the openGL window* to gain window focus,
- press the spacebar key repeatedly to simulate the scanline fill of the
	polygon provided in the question. 
The polygon, currently, is hard-coded for simplicity.


Question 2: 
	Source .cpp file = dpyoung_Assignment3_Q2.cpp
 	Visual Studio solution found in the folder Question2.
This program allows the user to view a Bunny.poly file (also found in the project
solution folder). User must first 
- *click in the openGL window* to gain focus
- Move the mouse in the openGL window to find the bunny model. 
- Pressing W or S will slide the camera forward or backward 
	(if the camera's upside-down, these buttons have an opposite effect). 
- The A and D keys strafe (if you want), 
- Q and E pitch the camera up or down,
- The T key toggles wireframe modeling vs. flat shading.
- The C key toggles a clipping plane.
- While the clipping plane is enabled, the following keys offer extensions:
- The Y and U keys pivot the clipping plane alone one of its axiis. 
Note that the model may not be in the viewing window at first; the camera depends
on the position of the mouse upon loading the window, and varies greatly when the 
mouse moves into the window (I have yet to get the mouse focus to stay in the
center of the screen). 