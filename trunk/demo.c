#include "polo.h"

void draw()
{
	int x = getMouseX();
	int y = getMouseY();
	int b = getMousePressed();

	int k = getKeyPressed();

	drawCircle(x, y, 100);
}

int main(int argc, char *argv[])
{
	// Init code here

	setGraphicsDrawCallback(draw);
	runGraphics(640, 480, 0);

	return 0;
}
