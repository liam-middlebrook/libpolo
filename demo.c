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
		
	setPenColor(POLO_TRANSPARENT);
	setFillColor(0x00000008);
	drawRect(0, 0, 640, 480);
	
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
	{
//		setPenColor(POLO_TRANSPARENT);
//		setGradientFillColors(0x00000040, 0x00000000);
//		drawCircle(getMouseX(), getMouseY(), 512);
		drawImage(getMouseX() - getImageWidth(d->image) / 2,
		          getMouseY() - getImageHeight(d->image) / 2,
		          d->image);
	}
	
	setPenColor(POLO_TUNGSTEN);
	setGradientFillColors(POLO_SILVER, POLO_TUNGSTEN);
//	drawRect(-1, -1, 128, getScreenHeight() + 2);
	drawRoundedRect(0, 0, 128, getScreenHeight(), 16);
	
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
	demoData.image = loadImage("demo.bmp");
	
	runPolo();
}

