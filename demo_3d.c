
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

#define DEMO_FIELD_OF_VIEW	45
#define DEMO_NEAR_PLANE		1
#define DEMO_FAR_PLANE		100

#define DEMO_CAMERA_X			0
#define DEMO_CAMERA_Y			0
#define DEMO_CAMERA_Z			4

#define DEMO_LOOKAT_X		0
#define DEMO_LOOKAT_Y		0
#define DEMO_LOOKAT_Z		0

#define DEMO_UP_X			0
#define DEMO_UP_Y			1
#define DEMO_UP_Z			0

#define LIGHT_ROT_FREQUENCY	0.25
#define LIGHT_RADIUS		2
#define LIGHT_POS_X			0
#define LIGHT_POS_Y			2
#define LIGHT_POS_Z			0

void draw(void *userData)
{
	float lightPos[4];
	
	float woodAmbient[4] = { 1.0, 1.0, 1.0, 1.0 };
	float woodDiffuse[4] = { 0.6, 0.4, 0.0, 1.0 };
	float woodSpecular[4] = { 1.0, 0.8, 0.4, 1.0 };
	float woodShininess = 50.0;
	
	float aspectRatio = (float) getScreenWidth() / getScreenHeight();
	
	/* Exit on key press */
	if (getKey())
		exitPolo();
	
	clearScreen();
	
	/* Set projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(DEMO_FIELD_OF_VIEW, aspectRatio, DEMO_NEAR_PLANE, DEMO_FAR_PLANE);
	
	/* Set camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(DEMO_CAMERA_X, DEMO_CAMERA_Y, DEMO_CAMERA_Z,
			  DEMO_LOOKAT_X, DEMO_LOOKAT_Y, DEMO_LOOKAT_Z,
			  DEMO_UP_X, DEMO_UP_Y, DEMO_UP_Z);
	
	/* Set rotating light around XZ plane */
	lightPos[0] = LIGHT_POS_X + LIGHT_RADIUS * cos(2.0 * M_PI * getTime() * LIGHT_ROT_FREQUENCY);
	lightPos[1] = LIGHT_POS_Y;
	lightPos[2] = LIGHT_POS_Z + LIGHT_RADIUS * sin(2.0 * M_PI * getTime() * LIGHT_ROT_FREQUENCY);
	lightPos[3] = 0;
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	/* Set material */
	glMaterialfv(GL_FRONT, GL_AMBIENT, woodAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, woodDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, woodSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, woodShininess);
	glColor4fv(woodDiffuse);
	
	/* Rotate camera with mouse (after setting the light) */
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
