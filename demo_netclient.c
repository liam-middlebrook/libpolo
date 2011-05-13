
/**
 * libpolonet demo
 * Simple connection demo
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 */

/*
 * Description:
 *
 * This demo attempts to download the main page of http://ik.itba.edu.ar
 */

#include <stdio.h>

#include "polonet.h"

int main(int argc, char *argv[])
{
	PolonetState state;
	char *command = "GET / HTTP/1.1\nHost:ik.itba.edu.ar\n\n";
	char buffer[256];
	int bytesSent;
	int bytesReceived;
	
	/* Open a connection */
	polonetOpenClient("ik.itba.edu.ar", 80);
	
	/* Wait for connection establishment */
	while((state = polonetGetState()) == POLONET_PENDING)
		usleep(10000); /* Wait 10 ms */
	
	if (state != POLONET_CONNECTED)
	{
		printf("Disconnected.\n");
		return 1;
	}
	
	/* Send the command */
	polonetSend(command, strlen(command));
	
	/* Receive the response */
	while(polonetGetState() == POLONET_CONNECTED)
	{
		int bytesReceived = polonetReceive(buffer, sizeof(buffer));
		if (bytesReceived)
			fwrite(buffer, bytesReceived, 1, stdout);
		else
			usleep(10000); /* Wait 10 ms */
	}
	
	return 0;
}

