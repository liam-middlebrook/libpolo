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
#include <unistd.h>

#include "polonet.h"

#define CONN_PORT 8080

char serverResponse[] =
"HTTP/1.1 200 OK\n"
"Server:libpolonet server demo/1.0 (Unknown)\n"
"Connection:close\n"
"Content-Type:text/html; charset=UTF-8\n"
"Content-Length:55\n"
"\n"
"<html><body><h1>libpolonet says hi!</h1></body></html>\n";

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
	/* Start listening on the port */
	if (!startListening(CONN_PORT))
		printf("Cannot listen on port %d.\n", CONN_PORT);
	else
	{
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
			
			/* Wait for the connection establishment.
			 If the connection is pending, wait 10 milliseconds */
			while (isPending(conn))
				usleep(10000);
			
			printf("Connected.\n");
			
			/* Wait for the client to send us something */
			if (getData(conn, buffer, sizeof(buffer)))
			{
				/* We ignore what the client sends,
				   and always return the same response */
				sendData(conn, serverResponse, strlen(serverResponse));
			}
			
			/* Always close an open connection! */
			closeConnection(conn);
		}
	}
	
	stopListening();
	
	return 0;
}
