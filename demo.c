#include <stdio.h>
#include "polo.h"

typedef struct
{
	int image;
} DemoData;

int frame = 0;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	char buf[256];
	
	if (getKey() == 27)
		exitPolo();
	
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image);
	
	setPenColor(POLO_STEEL);
	setGradientFillColors(POLO_SILVER, POLO_TUNGSTEN);
	drawRect(-1, -1, 128, getScreenHeight() + 2);
	
	setPenColor(POLO_BLACK);
	frame++;
	sprintf(buf, "FPS: %.3f", frame / getTime());
	drawText((128 - getTextDrawWidth(buf)) / 2,
			 getScreenHeight() - getTextDrawHeight(buf) - 10,
	         buf);
}

int main(int argc, char *argv[])
{
	DemoData demoData;
	
	initPolo(640, 480, 1, "Mouse Painter");
	
	setPoloUserData(&demoData);
	setDrawCallback(draw);
	setImageAlpha(0.1);
	demoData.image = loadImage("demo.bmp");
	
	runPolo();
}

