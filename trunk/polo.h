
/**
 * libpolo
 * Lightweight graphics library for educational environments
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Requires the GLUT library.
 */

/*
 * Usage:
 *
 * - Add polo.c and polo.h to your project.
 * - polo continuously calls a user function to redraw the screen.
 *   Call setDrawCallback() to define this function.
 * - Call runGraphics() to run graphics.
 * - To exit graphics, call exitGraphics() from your user function.
 * - You must change settings and draw from the user function.
 *
 * Coordinate system:
 * The coordinate system (0, 0) is at the lower left.
 */

#define Color int
#define Image int

// Initialization
void setDrawCallback(void *drawCallback(void));
void runGraphics(int width, int height, int fullscreen);

// Colors
Color getColorFromRGB(int red, int green, int blue);
Color getColorFromHSV(int hue, int saturation, int value);
Color getColorFromHSB(int hue, int saturation, int brightness);

void setPenColor(Color color);
void setFillColor(Color color1, Color color2);

// Drawing Primitives
void drawPoint(float x, float y);
void drawLine(float x1, float y1, float x2, float y2);
void drawRect(float x, float y, float width, float height);
void drawRoundedRect(float x, float y, float width, float height, float edgeRadius);
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void drawCircle(float x, float y, float radius);

// Text
enum PoloFont {
	POLO_COURIER_8,
	POLO_COURIER_9,
	POLO_TIMES_10,
	POLO_TIMES_24,
	POLO_HELVETICA_10,
	POLO_HELVETICA_12,
	POLO_HELVETICA_18,
};
void setTextFont(PoloFont font);
int getTextDrawWidth(char *str);
drawText(float x, float y, char *str);

// Images
Image loadImage(char *path);
void freeImage(Image image);
void drawImage(int x, int y, Image image);

// Mouse
float getMouseX();
float getMouseY();
float getMouseButtonState(int buttonIndex);

// Keyboard
int getKeyboardKey();

