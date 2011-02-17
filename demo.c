#include <stdio.h>
#include "polo.h"

typedef struct
{
	int image;
} DemoData;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	
	if (getKey() == 27)
		exitPolo();
	
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image);
}

int main(int argc, char *argv[])
{
	DemoData demoData;
	
	initPolo(640, 480, 0, "Mouse Painter");
	setPoloUserData(&demoData);
	
	setDrawCallback(draw);
	
	setImageAlpha(0.1);	
	demoData.image = loadImage("brush.bmp");
	
	runPolo();
}

