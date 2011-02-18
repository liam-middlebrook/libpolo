#include <stdio.h>
#include "polo.h"

typedef struct
{
	int image;
} DemoData;

void draw(void *userData)
{
	DemoData *d = (DemoData *) userData;
	const char *text = "Paleta de\n Colores";
	
	if (getKey() == 27)
		exitPolo();
	
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image);
	
	setPenColor(POLO_WHITE);
	setGradientFillColors(POLO_SILVER, POLO_TUNGSTEN);
	drawRect(-1, -1, 128, getScreenHeight() + 2);
	
	setPenColor(POLO_BLACK);
	drawText( (128-getTextDrawWidth(text))/2,
	          getScreenHeight()-getTextDrawHeight(text)-10,
	          text );
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
