
/**
 * libpolo
 * Lightweight graphics library for educational environments
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Requires the freeglut library.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
#define POLO_MAX_IMAGES		1024

typedef struct
{
	int texture;
	int textureWidth;
	int textureHeight;
	int width;
	int height;
} PoloImage;

typedef struct
{
	PoloImage images[POLO_MAX_IMAGES];
	void *userData;
	void (*drawCallback)(void *userData);
	void (*keyboardCallback)(void *userData, int key);
	void (*mouseMotionCallback)(void *userData, int x, int y);
	void (*mouseButtonCallback)(void *userData, int button, int state);
	void (*timerCallback)(void *userData, int id);
	Color penColor;
	Color fillColor1;
	Color fillColor2;
	void *font;
	int key;
	float mouseX;
	float mouseY;
	int mouseButtonState[POLO_MOUSEBUTTON_NUM];
} PoloState;

typedef struct
{
	unsigned char id[2];
	unsigned char size[4];
	unsigned char reserved[4];
	unsigned char pixelsOffset[4];
	unsigned char headerSize[4];
	unsigned char width[4];
	unsigned char height[4];
	unsigned char numberOfPlanes[2];
	unsigned char bitsPerPixel[2];
	unsigned char compression[4];
} PoloBMPHeader;

// Static variables
// Note: static variables should always be avoided. They are used here
//       because freeglut does not pass a user pointer to the callbacks.
static PoloState poloState;

// Private callbacks
static void drawCallback()
{
	// Draw stored texture
	
	if (poloState.drawCallback)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glTranslatef(-1.0, -1.0, 0.0);
		glScalef(2.0 / glutGet(GLUT_WINDOW_WIDTH),
		         2.0 / glutGet(GLUT_WINDOW_HEIGHT),
		         1.0);
		
		poloState.drawCallback(poloState.userData);
	}
	
	// Recover current output
	
	updateScreen();
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
	
	if (poloState.mouseMotionCallback)
		poloState.mouseMotionCallback(poloState.userData, x, y);
}

static void mouseMotionCallback(int x, int y)
{
	poloState.mouseX = x;
	poloState.mouseY = glutGet(GLUT_WINDOW_HEIGHT) - y;
	
	if (poloState.mouseMotionCallback)
		poloState.mouseMotionCallback(poloState.userData, x, y);
}

static void timerCallback(int value)
{
//	if (poloState.mouseMotionCallback)
//		poloState.mouseMotionCallback(poloState.userData, x, y);
}

// Initialization & exit
void setPoloUserData(void *userData)
{
	poloState.userData = userData;
}

void initPolo(int width, int height, int fullscreen, char *windowTitle)
{
	int i;
	
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
	glutCreateWindow(windowTitle);
	if (fullscreen)
		glutFullScreen();
	
	getRunTime();
	
	glutDisplayFunc(drawCallback);
	glutIdleFunc(drawCallback);
//	glutShapeFunc(shapeCallback);
	
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);
	
	glutMouseFunc(mouseButtonCallback);
	glutMotionFunc(mouseMotionCallback);
	glutPassiveMotionFunc(mouseMotionCallback);
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
}

void runPolo()
{
	glutMainLoop();
}

void exitPolo()
{
	glutLeaveMainLoop();
}

// Drawing

void setDrawCallback(void (*drawCallback)(void *userData))
{
	poloState.drawCallback = drawCallback;
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
		
		// sector 0 to 5
		h *= 6.0;
		// integer part of h (0,1,2,3,4,5 or 6)
		float i = floor(h);
		// factorial part of h (0 to 1)
		float f = h - i;
		
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
			// case 5 (or 6):
			default:
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

void drawPoint(float x, float y)
{
	glBegin(GL_POINTS);
	setPoloColor(poloState.penColor);
	glVertex2f(x + 0.5, y + 0.5);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	setPoloColor(poloState.penColor);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	glEnd();
}

void drawRect(float x, float y, float width, float height)
{
	if ((width < 0.0) || (height < 0.0))
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
	glVertex2f(x + 0.5, y + 0.5);
	glVertex2f(x + width - 0.5, y + 0.5);
	glVertex2f(x + width - 0.5, y + height - 0.5);
	glVertex2f(x + 0.5, y + height - 0.5);
	glVertex2f(x + 0.5, y + 0.5);
	glEnd();
}

void drawRoundedRect(float x, float y, float width, float height, float edgeRadius)
{
	drawRect(x, y, width, height);
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	glBegin(GL_TRIANGLES);
	setPoloColor(poloState.fillColor2);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x3 + 0.5, y3 + 0.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	glVertex2f(x3 + 0.5, y3 + 0.5);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	glBegin(GL_TRIANGLES);
	setPoloColor(poloState.fillColor2);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x3 + 0.5, y3 + 0.5);
	glVertex2f(x4 + 0.5, y4 + 0.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glVertex2f(x2 + 0.5, y2 + 0.5);
	glVertex2f(x3 + 0.5, y3 + 0.5);
	glVertex2f(x4 + 0.5, y4 + 0.5);
	glVertex2f(x1 + 0.5, y1 + 0.5);
	glEnd();
}

void drawCircle(float x, float y, float radius)
{
	int i;
	
	if (radius < 0.001)
		return;
	
	glBegin(GL_TRIANGLE_FAN);
	setPoloColor(poloState.fillColor1);
	glVertex2f(x, y);
	setPoloColor(poloState.fillColor2);
	for(i = 0; i <= 360;i++)
		glVertex2f(x + radius * cos(i * 2.0 * M_PI / 360.0 + 0.5),
		           y + radius * sin(i * 2.0 * M_PI / 360.0 + 0.5));
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	setPoloColor(poloState.penColor);
	for(i = 0; i <= 360;i++)
		glVertex2f(x + radius * cos(i * 2.0 * M_PI / 360.0 + 0.5),
		           y + radius * sin(i * 2.0 * M_PI / 360.0 + 0.5));
	glEnd();
}

void clearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void updateScreen()
{
	glutSwapBuffers();
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
	y -= glutBitmapHeight(poloState.font);
	y += getTextDrawHeight(str);
	// Fix text baseline... this code is actually a bit nasty :-/
	y += 5;
	glRasterPos2f(x, y);
	
	glutBitmapString(poloState.font, str);
}

static unsigned int getLittleEndianValue(unsigned char *value, int bits)
{
	if (bits == 16)
		return (value[0] << 0) +
		       (value[1] << 8);
	else if (bits == 32)
		return (value[0] << 0) +
		       (value[1] << 8) +
		       (value[2] << 16) +
		       (value[3] << 24);
	
	return 0;
}

static int getNextPowerOf2(int value)
{
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;
	
	return value;
}

static Image getFreeImage()
{
	Image i;
	
	for (i = 1; i < POLO_MAX_IMAGES; i++)
		if (!poloState.images[i].texture)
			return i;
	
	return 0;
}

Image loadImage(char *path)
{
	PoloBMPHeader bmpHeader;
	int pixelsOffset, width, height, numberOfPlanes, bytesPerPixel, compression;
	int valid = 1;
	Image image = 0;
	
	FILE *fp = fopen(path, "rb");
	if (!fp)
		return 0;
	
	if (fread(&bmpHeader, sizeof(bmpHeader), 1, fp) != 1)
		valid = 0;
	
	if ((bmpHeader.id[0] != 'B') || (bmpHeader.id[1] != 'M'))
		valid = 0;
	
	pixelsOffset = getLittleEndianValue(bmpHeader.pixelsOffset, 32);
	width = getLittleEndianValue(bmpHeader.width, 32);
	height = getLittleEndianValue(bmpHeader.height, 32);
	numberOfPlanes = getLittleEndianValue(bmpHeader.numberOfPlanes, 16);
	bytesPerPixel = getLittleEndianValue(bmpHeader.bitsPerPixel, 16) / 8;
	compression = getLittleEndianValue(bmpHeader.compression, 32);
	
	if ((width > 4096) || (height > 4096) || (numberOfPlanes != 1) ||
	    ((bytesPerPixel != 3) && (bytesPerPixel != 4)) ||
	    (compression != 0))
		valid = 0;
	
	if (valid)
	{
		int textureWidth = getNextPowerOf2(width);
		int textureHeight = getNextPowerOf2(height);
		
		char *p = calloc(textureWidth * textureHeight, bytesPerPixel);
		int y;
		
		if (p)
		{
			fseek(fp, pixelsOffset, SEEK_SET);
			
			for (y = 0; y < height; y++)
				fread(&p[y * textureWidth * bytesPerPixel], width * bytesPerPixel, 1, fp);
				
			image = getFreeImage();
			if (image)
			{
				PoloImage *poloImage = &poloState.images[image];
				
				glGenTextures(1, &(poloImage->texture));
				
				glBindTexture(GL_TEXTURE_2D, poloImage->texture);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
					     textureWidth, textureHeight,
					     0, (bytesPerPixel == 4) ? GL_BGRA : GL_BGR,
					     GL_UNSIGNED_BYTE, p);
				
				poloImage->textureWidth = textureWidth;
				poloImage->textureHeight = textureHeight;
				poloImage->width = width;
				poloImage->height = height;
			}
			
			free(p);
		}
	}
	
	fclose(fp);
	
	return image;
}

void drawImage(float x, float y, Image image)
{
	PoloImage *poloImage;
	float width, height;
	
	if (image >= POLO_MAX_IMAGES)
		return;
	
	poloImage = &poloState.images[image];
	
	if (!poloImage->texture)
		return;
	
	width = ((float) poloImage->width) / poloImage->textureWidth;
	height = ((float) poloImage->height) / poloImage->textureHeight;

	glBindTexture(GL_TEXTURE_2D, poloImage->texture);
	
	glColor4f(1, 1, 1, 1);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(width, 0);
	glVertex2f(x + poloImage->width, y);
	glTexCoord2f(width, height);
	glVertex2f(x + poloImage->width, y + poloImage->height);
	glTexCoord2f(0, height);
	glVertex2f(x, y + poloImage->height);
	glEnd();
}

void freeImage(Image image)
{
	if (image >= POLO_MAX_IMAGES)
		return;
	
	glDeleteTextures(1, &poloState.images[image].texture);
	
	poloState.images[image].texture = 0;
}

// Keyboard

void setKeyboardCallback(void (*keyboardCallback)(void *userData, int key))
{
	poloState.keyboardCallback = keyboardCallback;
}

int getPressedKey()
{
	int key = poloState.key;
	poloState.key = 0;
	
	return key;
}

// Mouse

void setMouseMotionCallback(void (*mouseMotionCallback)(void *userData, int x, int y))
{
	poloState.mouseMotionCallback = mouseMotionCallback;
}

void setMouseButtonCallback(void (*mouseButtonCallback)(void *userData, int button, int state))
{
	poloState.mouseButtonCallback = mouseButtonCallback;
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

void showMousePointer()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

void hideMousePointer()
{
	glutSetCursor(GLUT_CURSOR_NONE);
}

// Timer

void setTimerCallback(void (*timerCallback)(void *userData, int id))
{
	
}

void runTimer(int id, int milliseconds)
{

}

float getRunTime()
{
	return glutGet(GLUT_ELAPSED_TIME) * 0.001;
}

