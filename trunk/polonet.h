/**
 * libpolonet
 * Lightweight network module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 *     Agustin Perez Moreno
 * Released under the GPL
 */

/*
 * PoloNet manages TCP/IP connections.
 * It is capable of making and receiving connections.
 *
 * Usage as a client:
 * - Call polonetConnect() to connect to a host and port. This returns
 *   a PolonetConnection identifier. If the identifier is 0, the
 *   connection could not be established
 * - Call polonetGetState() to determine the state of a connection:
 *   - While the connection is being established, the state is POLONET_PENDING
 *   - While a connection is established, the state is POLONET_CONNECTED
 *   - If the connection was closed and there are no bytes to be sent
 *     or to be received, the state is POLONET_DISCONNECTED
 *   - If there was an error, the state is POLONET_ERROR
 * - To send bytes to a connection, call polonetSend(). It returns
 *   the number of bytes sent.
 * - To receive bytes from a conneciton, call polonetReceive(). It returns
 *   the number of bytes received.
 * - To close a connection, call polonetClose()
 * - Note: all calls are non-blocking
 *
 * Usage as a server:
 * - Call polonetListen() to start listening to connection request on a port.
 * - Call polonetStopListening() to stop listening to connection requests.
 * - To determine if there is an incoming connection, call
 *   polonetGetConnection().
 * - You can use
 */

#ifndef _LIBPOLONET_H
#define _LIBPOLONET_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int PolonetConnection;

typedef enum
{
	POLONET_PENDING,
	POLONET_CONNECTED,
	POLONET_DISCONNECTED,
	POLONET_ERROR,
} PolonetState;

/* Server functions */
void polonetListen(unsigned short port);
void polonetStopListening();

PolonetConnection polonetGetConnection();

/* Client functions */

void polonetConnect(char *hostname, unsigned short port);
void polonetClose();

PolonetState polonetGetState();

int polonetSend(char *buffer, const int bufferSize);
int polonetReceive(char *buffer, const int bufferSize);

#ifdef __cplusplus
}
#endif

#endif

