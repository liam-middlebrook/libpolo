
/**
 * libpolo
 * Lightweight graphics library for educational environments
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Requires the GLUT library.
 */

#include <stdio.h>




#include <math.h>

#include "polo.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Definitions
#define POLO_MOUSEBUTTON_NUM	3

typedef struct
{
	void *userData;
	void (*drawCallback)(void *userData);
	int width;
	int height;
	int isDrawing;
	int window;
	int key;
	int mouseX;
	int mouseY;
	int mouseButtonState[POLO_MOUSEBUTTON_NUM];
	Color penColor;
	Color fillColor1;
	Color fillColor2;
} PoloState;

// Note: static variables should always be avoided. They are used here
//       because GLUT does not pass a user pointer to the callbacks.
static PoloState poloState;

// callbacks
static void drawCallback()
{
	if (poloState.drawCallback)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glTranslatef(-1.0, -1.0, 0.0);
		glScalef(2.0 / poloState.width, 2.0 / poloState.height, 1.0);
		
		poloState.isDrawing = 1;
		poloState.drawCallback(poloState.userData);
		poloState.isDrawing = 0;
		
		glutSwapBuffers();
	}
}

static void keyboardCallback(unsigned char key, int x, int y)
{
	poloState.key = key;
}

static void specialCallback(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			poloState.key = POLO_F1;
			break;
		case GLUT_KEY_F2:
			poloState.key = POLO_F2;
			break;
		case GLUT_KEY_F3:
			poloState.key = POLO_F3;
			break;
		case GLUT_KEY_F4:
			poloState.key = POLO_F4;
			break;
		case GLUT_KEY_F5:
			poloState.key = POLO_F5;
			break;
		case GLUT_KEY_F6:
			poloState.key = POLO_F6;
			break;
		case GLUT_KEY_F7:
			poloState.key = POLO_F7;
			break;
		case GLUT_KEY_F8:
			poloState.key = POLO_F8;
			break;
		case GLUT_KEY_F9:
			poloState.key = POLO_F9;
			break;
		case GLUT_KEY_F10:
			poloState.key = POLO_F10;
			break;
		case GLUT_KEY_F11:
			poloState.key = POLO_F11;
			break;
		case GLUT_KEY_F12:
			poloState.key = POLO_F12;
			break;
		case GLUT_KEY_LEFT:
			poloState.key = POLO_LEFT;
			break;
		case GLUT_KEY_RIGHT:
			poloState.key = POLO_RIGHT;
			break;
		case GLUT_KEY_UP:
			poloState.key = POLO_UP;
			break;
		case GLUT_KEY_DOWN:
			poloState.key = POLO_DOWN;
			break;
		case GLUT_KEY_PAGE_UP:
			poloState.key = POLO_PAGEUP;
			break;
		case GLUT_KEY_PAGE_DOWN:
			poloState.key = POLO_PAGEDOWN;
			break;
		case GLUT_KEY_HOME:
			poloState.key = POLO_HOME;
			break;
		case GLUT_KEY_END:
			poloState.key = POLO_END;
			break;
	}
}

static void mouseButtonCallback(int button, int state, int x, int y)
{
	printf("button %d, %d\n", x, y);

	if (button < POLO_MOUSEBUTTON_NUM)
		poloState.mouseButtonState[button] = state;
}

static void mouseMotionCallback(int x, int y)
{
	printf("motion %d, %d\n", x, y);
	poloState.mouseX = x;
	poloState.mouseY = poloState.height - y;
}

void setUserData(void *userData)
{
	poloState.userData = userData;
}

void setPoloCallback(void (*poloCallback)(void *userData))
{
	poloState.drawCallback = poloCallback;
}

void runPolo(int width, int height, int fullscreen, char *windowTitle)
{
	int i;
	
	poloState.width = width;
	poloState.height = height;
	poloState.isDrawing = 0;
	poloState.window = 0;
	poloState.key = 0;
	poloState.mouseX = 0;
	poloState.mouseY = 0;
	for (i = 0; i < POLO_MOUSEBUTTON_NUM; i++)
		poloState.mouseButtonState[i] = 0;
	poloState.penColor = POLO_WHITE;
	poloState.fillColor1 = POLO_TRANSPARENT;
	poloState.fillColor2 = POLO_TRANSPARENT;
	
	int argc = 1;
	char *argv[2] = {"polo", NULL};
	glutInit(&argc, argv);
	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	poloState.window = glutCreateWindow(windowTitle);
	if (fullscreen)
		glutFullScreen();
	
	glutDisplayFunc(drawCallback);
	glutIdleFunc(drawCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutMouseFunc(mouseButtonCallback);
	glutMotionFunc(mouseMotionCallback);
	
	glutMainLoop();
}

void exitPolo()
{
	glutDestroyWindow(poloState.window);
	
	glutLeaveMainLoop();
}

Color getColorFromRGB(unsigned char red, unsigned char green, unsigned char blue)
{
	return ((((int)red) << 24) +
	        (((int)green) << 16) +
	        (((int)blue) << 8) +
	        0xff);
}

Color getColorFromHSV(unsigned char hue, unsigned char saturation, unsigned char value)
{
/*        int f;
        long p, q, t;
 
        if (saturation == 0)
		r = g = b = value;
	else
	{
		int f = ((hue % 60) * 255) / 60;
        h /= 60;
 
        p = (v * (256 – s))/256;
        q = (v * ( 256 – (s * f)/256 ))/256;
        t = (v * ( 256 – (s * ( 256 – f ))/256))/256;
 
        switch( h ) {
                case 0:
                        *r = v;
                        *g = t;
                        *b = p;
                        break;
                case 1:
                        *r = q;
                        *g = v;
                        *b = p;
                        break;
                case 2:
                        *r = p;
                        *g = v;
                        *b = t;
                        break;
                case 3:
                        *r = p;
                        *g = q;
                        *b = v;
                        break;
                case 4:
                        *r = t;
                        *g = p;
                        *b = v;
                        break;
                default:
                        *r = v;
                        *g = p;
                        *b = q;
                        break;
        }*/
	return 0;
}

Color getColorFromHSB(unsigned char hue, unsigned char saturation, unsigned char brightness)
{
	return 0;
}

void setPenColor(Color color)
{
	poloState.penColor = color;
}

void setFillColor(Color color)
{
	poloState.fillColor1 = color;
	poloState.fillColor2 = color;
}

void setGradientFillColors(Color color1, Color color2)
{
	poloState.fillColor1 = color1;
	poloState.fillColor2 = color2;
}

static void setPoloColor(Color color)
{
	glColor4f(((color & 0xff000000) >> 24) / 255.0,
	          ((color & 0x00ff0000) >> 16) / 255.0,
	          ((color & 0x0000ff00) >> 8) / 255.0,
	          ((color & 0x000000ff) >> 0) / 255.0);
}

void clearScreen()
{
	if (!poloState.isDrawing)
		return;
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawPoint(float x, float y)
{
	if (!poloState.isDrawing)
		return;
	
	glBegin(GL_POINTS);
	setPoloColor(poloState.penColor);
	glVertex2f(x, y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	if (!poloState.isDrawing)
		return;
	
	glBegin(GL_LINES);
	setPoloColor(poloState.penColor);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void drawRect(float x, float y, float width, float height)
{
	if (!poloState.isDrawing)
		return;
	
	glBegin(GL_QUADS);
	setPoloColor(poloState.fillColor2);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glVertex2f(x, y);
	glEnd();
}

void drawRoundedRect(float x, float y, float width, float height, float edgeRadius)
{
	if (!poloState.isDrawing)
		return;
	
	drawRect(x, y, width, height);
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	if (!poloState.isDrawing)
		return;
	
	glBegin(GL_TRIANGLES);
	setPoloColor(poloState.fillColor2);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x3, y3);
	glEnd();

	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	if (!poloState.isDrawing)
		return;
	
	glBegin(GL_TRIANGLES);
	setPoloColor(poloState.fillColor2);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();

	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glVertex2f(x1, y1);
	glEnd();
}

void drawCircle(float x, float y, float radius)
{
	int i;
	
	if (!poloState.isDrawing)
		return;
	
	if (radius < 1.0)
		return;
	
	glBegin(GL_TRIANGLE_FAN);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x, y);
	setPoloColor(poloState.fillColor2);
	for(i = 0; i <= 360;i++)
		glVertex2f(x + radius * cos(i * 2.0 * M_PI / 360.0),
		           y + radius * sin(i * 2.0 * M_PI / 360.0));
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	for(i = 0; i <= 360;i++)
		glVertex2f(x + radius * cos(i * 2.0 * M_PI / 360.0),
		           y + radius * sin(i * 2.0 * M_PI / 360.0));
	glEnd();
}

void setTextFont(enum PoloFont font)
{
}

int getTextDrawWidth(char *str)
{
	return 0;
}

void drawText(float x, float y, char *str)
{
}

Image loadImage(char *path)
{
	return 0;
}

void freeImage(Image image)
{
}

void drawImage(float x, float y, Image image)
{
	if (!poloState.isDrawing)
		return;
}

float getMouseX()
{
	return poloState.mouseX;
}

float getMouseY()
{
	return poloState.mouseY;
}

int isMouseButtonPressed(int buttonIndex)
{
	if (buttonIndex >= POLO_MOUSEBUTTON_NUM)
		return 0;
	
	return poloState.mouseButtonState[buttonIndex];
}

int getPressedKey()
{
	int key = poloState.key;
	poloState.key = 0;
	
	return key;
}

void setKeyboardCallback(void *keyCallback(void *userData, int key), void *userData)
{
}

void setMouseMotionCallback(void *keyCallback(void *userData, int x, int y), void *userData)
{
}

void setMouseButtonCallback(void *keyCallback(void *userData, int button, int state), void *userData)
{

}

float getRunTime()
{
	return 0.0;
}

void showMousePointer()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void hideMousePointer()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}

