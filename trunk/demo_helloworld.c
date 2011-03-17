/**
 * libpolo hello world demo
 * Hello world program
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 *
 * Requires libpolo.
 */

#include <stdio.h>
#include "polo.h"

void draw(void *userData)
{
	/* Draw rectangle */
	setPenColor(POLO_TRANSPARENT);
	setFillColor(POLO_MIDNIGHT);
	drawRect(0, 0, 120, 44);
	
	/* Draw hello world */
	setPenColor(POLO_WHITE);
	drawText(10, 10, "Hello world");
}

int main(int argc, char *argv[])
{
	/* Init polo */
	initPolo(640, 480, 0, "Hello world");
	
	/* Init variables */
	setDrawCallback(draw);
	
	/* Run polo */
	runPolo();
	
	return 0;
}

