
/**
 * libpolonet server demo
 * Simple server demo
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 * Released under the GPL
 */

/*
 * Description:
 *
 * Responds to port 8080, serves a simple webpage.
 *
 * Test with this URL on your browser: http://localhost:8080
 */

#include <stdio.h>
#include <string.h>

#include "polonet.h"

char serverResponse[] =
"HTTP/1.1 200 OK\n"
"Server: libpolonet server demo/1.0 (Unknown)\n"
"Content-Length: 45\n"
"Connection: close\n"
"Content-Type: text/html; charset=UTF-8\n"
"\n"
"<html><body><h1>libpolonet says hi!</h1></body></html>\n";

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
	/* Start listening on the port */
	startListening(8080);
	
	printf("Server started.\n");
	
	/* Well-behaved servers always keep listening */
	while (1)
	{
		PolonetConn conn;
		char buffer[256];
		
		/* Wait for an incoming connection.
		   If there is no connection, wait 10 milliseconds */
		while (!(conn = getAvailableConnection()))
			usleep(10000);

		printf("Connected.\n");
		
		/* Wait for the client to send us something */
		if (!getData(conn, buffer, sizeof(buffer)))
			continue;
		
		/* We ignore what the client sends,
		   and always return the same response */
		sendData(conn, serverResponse, strlen(serverResponse));
		
		closeConnection(conn);
	}
	
	stopListening();
	
	return 0;
}

