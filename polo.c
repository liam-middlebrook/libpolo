
/**
 * libpolo
 * Lightweight graphics library for educational environments
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Requires the GLUT library.
 */

#include <math.h>

#include "polo.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/freeglut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
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
	Color penColor;
	Color fillColor1;
	Color fillColor2;
	void *font;
	int key;
	float mouseX;
	float mouseY;
	int mouseButtonState[POLO_MOUSEBUTTON_NUM];
} PoloState;

// Note: static variables should always be avoided. They are used here
//       because GLUT does not pass a user pointer to the callbacks.
static PoloState poloState;

// Callbacks
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
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			button = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			button = 1;
			break;
		case GLUT_MIDDLE_BUTTON:
			button = 2;
			break;
		default:
			return;
	}
	
	if (state == GLUT_DOWN)
		poloState.mouseButtonState[button] = 1;
	else if (state == GLUT_UP)
		poloState.mouseButtonState[button] = 0;
	
	poloState.mouseX = x;
	poloState.mouseY = poloState.height - y;
}

static void mouseMotionCallback(int x, int y)
{
	poloState.mouseX = x;
	poloState.mouseY = poloState.height - y;
}

// Initialization & exit
void setUserData(void *userData)
{
	poloState.userData = userData;
}

void setPoloCallback(void (*poloCallback)(void *userData))
{
	poloState.drawCallback = poloCallback;
}

void initPolo(int width, int height, int fullscreen, char *windowTitle)
{
	int i;
	
	poloState.width = width;
	poloState.height = height;
	poloState.isDrawing = 0;
	poloState.window = 0;
	setPenColor(POLO_WHITE);
	setFillColor(POLO_TRANSPARENT);
	setTextFont(POLO_HELVETICA_18);
	poloState.key = 0;
	poloState.mouseX = 0;
	poloState.mouseY = 0;
	for (i = 0; i < POLO_MOUSEBUTTON_NUM; i++)
		poloState.mouseButtonState[i] = 0;
	
	int argc = 1;
	char *argv[2] = {"polo", NULL};
	glutInit(&argc, argv);
	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	poloState.window = glutCreateWindow(windowTitle);
	if (fullscreen)
		glutFullScreen();
	
	getRunTime();
	
	glutDisplayFunc(drawCallback);
	glutIdleFunc(drawCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	glutMouseFunc(mouseButtonCallback);
	glutMotionFunc(mouseMotionCallback);
	glutPassiveMotionFunc(mouseMotionCallback);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutSwapBuffers();
}

void runPolo()
{
	glutMainLoop();
}

void exitPolo()
{
	glutDestroyWindow(poloState.window);
	
	glutLeaveMainLoop();
}

static float saturate(float value)
{
	if (value < 0.0)
		return 0.0;
	if (value > 1.0)
		return 1.0;
	return value;
}

Color getColorFromRGB(float red, float green, float blue)
{
	float r = saturate(red);
	float g = saturate(green);
	float b = saturate(blue);

	return ((((int)(r * 255.0)) << 24) +
	        (((int)(g * 255.0)) << 16) +
	        (((int)(b * 255.0)) << 8) +
	        0xff);
}

Color getColorFromHSV(float hue, float saturation, float value)
{
	float r, g, b;
	
	float h = hue - floor(hue);
	float s = saturate(saturation);
	float v = saturate(value);
	
	if (s == 0.0)
		r = g = b = v;
	else
	{
		// If hue == 1.0, then wrap it around the circle to 0.0
		if (h == 1.0)
			h = 0.0;
		
		h *= 6.0;			// sector 0 to 5
		float i = floor(h);		// integer part of h (0,1,2,3,4,5 or 6)
		float f = h - i;		// factorial part of h (0 to 1)
		
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));
		
		switch ((int) h)
		{
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			default:		// case 5 (or 6):
				r = v;
				g = p;
				b = q;
				break;
		}
	}
	
	return getColorFromRGB(r, g, b);
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
	
	if (radius < 0.001)
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
	switch (font)
	{
		case POLO_COURIER_13:
			poloState.font = GLUT_BITMAP_8_BY_13;
			break;
		case POLO_COURIER_15:
			poloState.font = GLUT_BITMAP_9_BY_15;
			break;
		case POLO_TIMES_10:
			poloState.font = GLUT_BITMAP_TIMES_ROMAN_10;
			break;
		case POLO_TIMES_24:
			poloState.font = GLUT_BITMAP_TIMES_ROMAN_24;
			break;
		case POLO_HELVETICA_10:
			poloState.font = GLUT_BITMAP_HELVETICA_10;
			break;
		case POLO_HELVETICA_12:
			poloState.font = GLUT_BITMAP_HELVETICA_12;
			break;
		case POLO_HELVETICA_18:
		default:
			poloState.font = GLUT_BITMAP_HELVETICA_18;
			break;
	}
}

float getTextDrawWidth(char *str)
{
	return glutBitmapLength(poloState.font, str);
}

float getTextDrawHeight(char *str)
{
	int i = 1;
	while (*str)
	{
		if (*str == '\n')
			i++;
		
		str++;
	}
	
	return i * glutBitmapHeight(poloState.font);
}

void drawText(float x, float y, char *str)
{
	y += getTextDrawHeight(str);
	
	y += 5;
	y -= glutBitmapHeight(poloState.font);
	glRasterPos2f(x, y);
	while(*str)
	{
		if (*str == '\n')
		{
			y -= glutBitmapHeight(poloState.font);
			glRasterPos2f(x, y);
		}
		else
			glutBitmapCharacter(poloState.font, *str);
		
		str++;
	}
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
	return glutGet(GLUT_ELAPSED_TIME) * 0.001;
}

void showMousePointer()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void hideMousePointer()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}

