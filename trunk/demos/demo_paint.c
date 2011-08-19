/**
 * libpolo paint demo
 * Simple paint program
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 * Released under the GPL
 *
 * Requires libpolo.
 */

#include <stdio.h>
#include <polo/polo.h>

#define TOOLBAR_WIDTH 130
#define BRUSH_ALPHA 0.2

typedef struct
{
	Image brush;
	int frame;
} DemoData;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	char buf[256];
	int x, y;
	
	/* Exit on key press */
	if (getKey())
		exitPolo();
	
	/* Clear screen with right mouse button */
	if (isMouseButtonPressed(1))
		clearScreen();
	
	/* Paint with left mouse button */
	if (isMouseButtonPressed(0))
	{
		if ((getMouseX() < 128) && (getMouseY() < 128))
			setDrawTint(getColorFromHSVA(getMouseX() / 128,
			                             getMouseY() / 128,
			                             1,
			                             BRUSH_ALPHA));
		else
			drawImage(getMouseX() - getImageWidth(d->brush) / 2,
			          getMouseY() - getImageHeight(d->brush) / 2,
			          d->brush);
	}
	
	/* Draw left bar */
	setPenColor(POLO_STEEL);
	setFillGradient(POLO_SILVER, POLO_TUNGSTEN);
	drawRect(-1, -1, TOOLBAR_WIDTH, getScreenHeight() + 2);
	
	/* Draw frames per second and time display */
	setPenColor(POLO_BLACK);
	sprintf(buf, "FPS: %.3f", d->frame / (getTime() + 0.001));
	drawText((TOOLBAR_WIDTH - getTextDrawWidth(buf)) / 2,
	         getScreenHeight() - getTextDrawHeight(buf) - 10,
	         buf);
	
	/* Increment frame number */
	d->frame++;
	
	/* Paint color palette */
	for (x = 0; x < 128; x++)
		for (y = 0; y < 128; y++)
		{
			setPenColor(getColorFromHSV(x / 128.0,
			                            y / 128.0,
			                            1.0));
			drawPoint(x, y);
		}
}

int main(int argc, char *argv[])
{
	DemoData demoData;
	
	/* Init polo */
	initPolo(640, 480, 0, "Mouse Painter");
	
	/* Init variables */
	setPoloUserData(&demoData);
	setDrawCallback(draw);
	setDrawTint(getColorFromHSVA(0.0, 0.0, 1.0, BRUSH_ALPHA));
	demoData.brush = loadImage("brush.bmp");
	demoData.frame = 0;
	
	/* Run polo */
	runPolo();
	
	return 0;
}

