#include <stdio.h>
#include "polo.h"

void draw(void *userData)
{
	int x, y;

	setPenColor(POLO_TRANSPARENT);
	setFillColor(POLO_CANTALOUPE);
	for (x = 0; x < 640; x++)
		for (y = 0; y < 480; y++)
		{
			setPenColor(getColorFromHSV(x * x / 320.0 / 320.0-0.5, y / 240.0-0.5, 1.0));
			drawPoint(x, y);
		}
}

int main(int argc, char *argv[])
{
	initPolo(640, 480, 0, "Mouse Painter");

	setPoloCallback(draw);
	draw(NULL);

	runPolo();
}
	
