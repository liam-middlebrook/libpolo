
/**
 * libpolonet server demo
 * Simple server demo
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 * Released under the GPL
 */

/*
 * Description:
 *
 * Responds to port 8080, serves a simple webpage.
 */

#include <stdio.h>

#include "polonet.h"

int main(int argc, char *argv[])
{
	PolonetState state;
	char *response =
"HTTP/1.1 200 OK\n"
"Server: libpolonet server demo/1.0 (Unknown)\n"
"Content-Length: 45\n"
"Connection: close\n"
"Content-Type: text/html; charset=UTF-8\n"
"\n"
"<html><body>libpolonet says hi!</body></html>\n";
	
	char buffer[256];
	int bytesSent;
	int bytesReceived;
	
	/* Servers always implement a listen loop */
	while (1)
	{
		state = polonetOpenServer(8080);
		
		while((state = polonetGetState()) == POLONET_PENDING)
			usleep(100000);
		
		if (state == POLONET_ERROR)
			break;
		else if (state != POLONET_CONNECTED)
			continue;
		
		printf("Connected.\n");
		
		/* Wait for the client to send us something */
		int a;
		while((a = polonetReceive(buffer, sizeof(buffer) - 1, &bytesReceived)
			  ) == POLONET_CONNECTED)
		{
			if (!bytesReceived)
				usleep(100000);
			else
				break;
		}

		printf("%d\n", a);

		/*
		 * We ignore the commands the clients sends to us,
		 * and send the same response always.
		 */
		
		polonetSend(response, sizeof(response) - 1, &bytesSent);
		
		sleep(1);

		polonetClose();
	}
	
	printf("Error.\n");
	
	polonetClose();
	
	return 0;
}

