
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
#include "polo.h"

typedef struct
{
	int image;
	int frame;
} DemoData;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	char buf[256];
	
	// Exit when ESC is pressed
	if (getKey() == 27)
		exitPolo();
	
	// Clear screen with right mouse button
	if (isMouseButtonPressed(1))
		clearScreen();
	// Paint with left mouse button
	if (isMouseButtonPressed(0))
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image, getColorFromRGBA(1, 1, 1, 0.1));
	
	// Draw left bar
	setPenColor(POLO_STEEL);
	setFillGradient(POLO_SILVER, POLO_TUNGSTEN);
	drawRect(-1, -1, 128, getScreenHeight() + 2);
	
	// Draw frames per second display
	setPenColor(POLO_BLACK);
	sprintf(buf, "FPS: %.3f", d->frame / (getTime() + 0.001));
	drawText((128 - getTextDrawWidth(buf)) / 2,
			 getScreenHeight() - getTextDrawHeight(buf) - 10,
	         buf);
	
	// Increment frame number
	d->frame++;
}

int main(int argc, char *argv[])
{
	DemoData demoData;
	
	// Init polo
	initPolo(640, 480, 0, "Mouse Painter");
	
	// Init our variables
	setPoloUserData(&demoData);
	setDrawCallback(draw);
	demoData.image = loadImage("brush.bmp");
	demoData.frame = 0;
	
	// Run polo
	runPolo();
}
