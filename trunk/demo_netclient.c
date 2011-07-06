/**
 * libpolonet client demo
 * Simple client demo
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 * Released under the GPL
 */

/*
 * Description:
 *
 * This demo attempts to download the main page of http://www.google.com
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "polonet.h"

#define CONN_HOSTNAME "www.google.com"
#define CONN_PORT 80

char getCommand[] =
"GET / HTTP/1.1\n"
"Host:" CONN_HOSTNAME "\n"
"\n";

/* Blocking receiveData. It waits til data is received. */
int getData(PolonetConn conn, char *buffer, int buffersize)
{
	while (isConnected(conn))
	{
		int bytesReceived;
		
		if (bytesReceived = receiveData(conn, buffer, buffersize))
			return bytesReceived;
		
		/* Wait 10 milliseconds, so the CPU is not clogged */
		usleep(10000);
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	PolonetConn conn;
	char buffer[256];
	int bytesReceived;
	int returnValue = 1;
	
	/* Open the connection */
	if ((conn = openConnection(CONN_HOSTNAME, 80)))
	{
		/* Wait for the connection establishment.
		   If the connection is pending, wait 10 milliseconds */
		while (isPending(conn))
			usleep(10000);
		
		if (isConnected(conn))
		{
			/* Send the GET command */
			sendData(conn, getCommand, strlen(getCommand));
			
			/* Receive the response */
			while (bytesReceived = getData(conn, buffer, sizeof(buffer) - 1))
			{
				buffer[bytesReceived] = '\0';
				printf("%s", buffer);
			}
			
			/* Signal success */
			returnValue = 0;
		}
		else
			printf("Connection refused from " CONN_HOSTNAME ".\n");
		
		/* Always close an open connection! */
		closeConnection(conn);
	}
	else
		printf("Couldn't connect to " CONN_HOSTNAME ".\n");
	
	return returnValue;
}

