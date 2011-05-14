/**
 * libpolonet
 * Lightweight TCP/IP network module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 *     Agustin Perez Moreno
 * Released under the GPL
 */

/*
 * Polonet is capable of making and receiving TCP/IP connections.
 *
 * Usage as a client:
 * - Call openConnection() to connect to a host. This returns a
 *   PolonetConn identifier. If the identifier is 0, the connection
 *   could not be established
 *
 * Usage as a server:
 * - Call startListening() to start listening to connections on a port.
 * - getAvailableConnection() gets the next available incoming connection.
 *   If there is no incoming connection, it returns 0.
 * - You can use the client functions to handle the incoming connection.
 *
 * Managing a connection:
 * - isPending() determines if a connection is being established.
 * - isConnected() determines if a connection is established.
 * - Call sendData() to send bytes to a connection. It returns
 *   the number of bytes sent.
 * - Call receiveData() to receive bytes from a connection. It returns
 *   the number of bytes received.
 * - Call closeConnection() to close a connection
 * - Note: all calls are non-blocking
 * - Note: notice there might be data left for reading, even though a
 *   connection is closed.
 */

#ifndef _LIBPOLONET_H
#define _LIBPOLONET_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int PolonetConn;

/* Client functions */
PolonetConn openConnection(char *hostname, unsigned short port);

/* Server functions */
int startListening(unsigned short port);
void stopListening();
PolonetConn getAvailableConnection();

/* Connection functions */
int isPending(PolonetConn conn);
int isConnected(PolonetConn conn);
int sendData(PolonetConn conn, char *buffer, const int bufferSize);
int receiveData(PolonetConn conn, char *buffer, const int bufferSize);
void closeConnection(PolonetConn conn);

#ifdef __cplusplus
}
#endif

#endif
