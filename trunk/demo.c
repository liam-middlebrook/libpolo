#include <stdio.h>
#include "polo.h"

typedef struct
{
	int image;
} DemoData;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	
	static int t = 0;
	
	if (getKey() == 27)
		exitPolo();
	
/*	setPenColor(POLO_TRANSPARENT);
	setGradientFillColors(POLO_IRON, POLO_WHITE);
	drawRect(0, 0, 80, 480);
*/	
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image);
	
	if (t < 2)
	{
//	setGradientFillColors(POLO_CANTALOUPE, POLO_MARASCHINO);
	setPenColor(POLO_TRANSPARENT);
	setFillColor((t % 2) ? POLO_WHITE : POLO_BLACK);
	drawRect(0, 0, 640, 480);
	}
	
	t++;
}

int main(int argc, char *argv[])
{
	DemoData demoData;
	
	initPolo(640, 480, 0, "Mouse Painter");
	setPoloUserData(&demoData);
	
	setDrawCallback(draw);
	setImageAlpha(0.1);
	
	demoData.image = loadImage("demo.bmp");
	
	runPolo();
}

