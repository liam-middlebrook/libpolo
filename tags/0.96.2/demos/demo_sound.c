/**
 * libpolosnd demo
 * Sound demo program
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 *
 * Requires libpolosnd.
 */

#include <stdio.h>
#include <unistd.h>

#include <polo/polosnd.h>

int main(int argc, char *argv[])
{
	/* Open the sound device */
	openSound();
	
	/* Play back a sound */
	playSound("demo_sound.ogg");
	usleep(200000);
	
	/* Wait a second */
	stopSound();
	usleep(500000);
	
	/* Play again */
	playSound("demo_sound.ogg");
	sleep(2);
	
	/* Close the sound device */
	closeSound();
	
	return 0;
}

