#include <stdio.h>
#include "polo.h"

int image = 0;

void draw(void *userData)
{
	if (isMouseButtonPressed(1))
		clearScreen();
	if (isMouseButtonPressed(0))
		drawImage(getMouseX(), getMouseY(), image);
}

int main(int argc, char *argv[])
{
	initPolo(640, 480, 0, "Mouse Painter");
	setDrawCallback(draw);
	image = loadImage("demo.bmp");
	runPolo();
}

