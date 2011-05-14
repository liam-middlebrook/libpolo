
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

#include "polonet.h"

#define HOSTNAME "www.google.com"

char getCommand[] =
"GET / HTTP/1.1\n"
"Host:" HOSTNAME "\n"
"\n";

/* Convenience method to receive data */
int getData(PolonetConn conn, char *buffer, int buffersize)
{
	int bytesReceived;
	
	while (isConnected(conn))
	{
		if (bytesReceived = receiveData(conn, buffer, buffersize))
			return bytesReceived;
		
		usleep(10000);
	}
	
	/* Just in case there is data left */ 
	if (bytesReceived = receiveData(conn, buffer, buffersize))
		return bytesReceived;
	
	return 0;
}

int main(int argc, char *argv[])
{
	PolonetConn conn;
	char buffer[256];
	int bytesReceived;
	
	/* Open the connection */
	if (!(conn = openConnection(HOSTNAME, 80)))
	{
		printf("Couldn't connect to " HOSTNAME ".\n");
		return 1;
	}
	
	/* Wait for the connection establishment.
	   If the connection is pending, wait 10 milliseconds */
	while (isPending(conn))
		usleep(10000);
	
	if (!isConnected(conn))
	{
		printf("Disconnected from " HOSTNAME ".\n");
		return 1;
	}
	
	/* Send the GET command */
	sendData(conn, getCommand, strlen(getCommand));
	
	/* Receive the response */
	while (bytesReceived = getData(conn, buffer, sizeof(buffer) - 1))
	{
		buffer[bytesReceived] = '\0';
		printf("%s", buffer);
	}
	
	closeConnection(conn);
	
	return 0;
}

