#include <stdio.h>
#include "polo.h"

void draw(void *userData)
{
//	clearScreen();
	setFillColor(POLO_CANTALOUPE);
//	setFillColor(isMouseButtonPressed(0) ? POLO_WHITE : POLO_TRANSPARENT);
	drawCircle(getMouseX(), getMouseY(), 5.0);
}

int main(int argc, char *argv[])
{
	setPoloCallback(draw);
	runPolo(640, 480, 1, "Mouse Painter");

	printf("Chau\n");

	return 0;
}

