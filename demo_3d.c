
/**
 * libpolo 3D demo
 * Simple 3D demo (integration with OpenGL)
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 *
 * Requires libpolo.
 */

#include <stdio.h>
#include <math.h>

#include "polo.h"

void draw(void *userData)
{
	GLfloat lightPos[4] = { 0.0, 4.0, 0.0, 0.0 };
	
	float woodAmbient[4] = { 1.0, 1.0, 1.0, 1.0 };
	float woodDiffuse[4] = { 0.6, 0.4, 0.0, 1.0 };
	float woodSpecular[4] = { 1.0, 0.8, 0.4, 1.0 };
	
	float fieldOfView = 45;
	float aspectRatio = (float) getScreenWidth() / getScreenHeight();
	float nearPlane = 1;
	float farPlane = 100;
	
	/* Exit on key press */
	if (getKey())
		exitPolo();
	
	clearScreen();
	
	/* Set projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	
	/* Set camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4,
			  0, 0, 0,
			  0, 1, 0);
	
	/* Set rotating light around XZ plane */
	lightPos[0] = 2.0 * cos(2.0 * M_PI * getTime() * 0.25);
	lightPos[2] = 2.0 * sin(2.0 * M_PI * getTime() * 0.25);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	/* Set material */
	glMaterialfv(GL_FRONT, GL_AMBIENT, woodAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woodDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, woodSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glColor4fv(woodDiffuse);
	
	/* Rotate with mouse (after setting the light) */
	glRotatef(225 - 360.0 * getMouseY() / getScreenHeight(), 1, 0, 0);
	glRotatef(135 + 360.0 * getMouseX() / getScreenWidth(), 0, 1, 0);
	
	/* Draw object */
	if (isMouseButtonPressed(0))
		glutWireTeapot(1);
	else
		glutSolidTeapot(1.0);
}

int main(int argc, char *argv[])
{
	GLfloat position[] = {0.0, 0.0, 1.5, 1.0};
	
	/* Init polo */
	initPolo(640, 480, 0, "Teapot Demo");
	
	/* Init variables */
	setDrawCallback(draw);
	
	/* Configure OpenGL */
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	setTexture(loadImage("wood.bmp"));
	
	/* Run polo */
	runPolo();
}
