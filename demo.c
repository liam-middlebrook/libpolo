#include "polo.h"

void draw()
{
	setFillColor(isMouseButtonPressed() ? POLO_WHITE : POLO_TRANSPARENT);
	drawCircle(getMouseX(), getMouseY(), 50);
}

int main(int argc, char *argv[])
{
	setGraphicsDrawCallback(draw);
	runGraphics(640, 480, 0, "Mouse Painter");

	return 0;
}

