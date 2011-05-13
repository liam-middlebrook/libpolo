/**
 * libpolonet
 * Lightweight network module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 *     Agustin Perez Moreno
 * Released under the GPL
 */

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <string.h>

#include "polonet.h"

static int isError = 0;
static int fdserver = -1;
static int fdclient = -1;

static int polonetInit()
{
#ifdef WIN32
	static int isWSAInitialized = 0;
	
	if (!isWSAInitialized)
	{
		WSADATA wsaData;
		WORD wsaVersion = MAKEWORD(2, 0);
		
		if (WSAStartup(wsaVersion, &wsaData))
			return 0;
		
		isWSAInitialized = 1;
	}
#endif
	
	return 1;
}

static int polonetSetNonBlocking(int fd)
{
#ifdef WIN32
	unsigned long isNonBlocking = 1;
	return ioctlsocket(fdserver, FIONBIO, &isNonBlocking) != -1;
#else
	int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
		return 0;
	
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
#endif
}

static int polonetSetReuseAddress(int fd)
{
#ifndef WIN32
	int value = 1;
	if (setsockopt(fdserver, SOL_SOCKET, SO_REUSEADDR,
	    &value, sizeof(value) == -1)
		return 0;
#endif
	
	return 1;
}

static void polonetCloseSocket(int *fd)
{
	if (*fd == -1)
		return;
#ifdef WIN32
	closesocket(*fd);
#else
	close(*fd);
#endif
	*fd = -1;
}

/*
 * Public functions
 */

void polonetOpenClient(char *hostname, unsigned short port)
{
	struct hostent * host;
	struct sockaddr_in sin;
	
	polonetInit();
	
	polonetCloseSocket(&fdclient);
	
	if ((host = gethostbyname(hostname)) == 0)
		return POLONET_ERROR;
	
	if ((fdclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return POLONET_ERROR;
	
	polonetSetNonBlocking(fdclient);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ((struct in_addr *) (host->h_addr))->s_addr;
	sin.sin_port = htons(port);
	
	connect(fdclient, (struct sockaddr *) &sin, sizeof(sin));
	
#ifdef WIN32
	if (WSAGetLastError() != WSAEWOULDBLOCK)
		return POLONET_ERROR;
#endif
	return POLONET_PENDING;
}

void polonetOpenServer(unsigned short port)
{
	polonetCloseSocket(&fdserver);
	
	if ((fdserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		isError = 1;
		return;
	}
	
	polonetSetNonBlocking(fdserver);
	polonetSetReuseAddress(fdserver);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr =  htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	
	if (bind(fdserver, (struct sockaddr *) &sin, sizeof(sin)) == -1)
	{
		isError = 1;
		return;
	}
	
	if (listen(fdserver, 0) == -1)
	{
		isError = 1;
		return;
	}
}

PolonetState polonetGetState()
{
	fd_set fdsRead;
	fd_set fdsWrite;
	fd_set fdsExcept;
	struct timeval nowait;
	
	if (isError)
	{
		isError = 0;
		return POLONET_ERROR;
	}
	
	/* Check server connection */
	
	memset((char *)&nowait, 0, sizeof(nowait));
	
	if ((fdclient == -1) && (fdserver != -1))
	{
		FD_ZERO(&fdsRead);
		FD_SET((unsigned int) fdserver, &fdsRead);
		
		if (select(fdserver + 1, &fdsRead, NULL, NULL, &nowait) == -1)
			return POLONET_ERROR;
		
		if(FD_ISSET(fdserver, &fdsRead))
			fdclient = accept(fdserver, NULL, NULL);
		else
			return POLONET_PENDING;
	}
	
	/* Check client connection */
	
	if (fdclient == -1)
		return POLONET_DISCONNECTED;
	
	FD_ZERO(&fdsRead);
	FD_SET((unsigned int) fdclient, &fdsRead);
	FD_ZERO(&fdsWrite);
	FD_SET((unsigned int) fdclient, &fdsWrite);
	FD_ZERO(&fdsExcept);
	FD_SET((unsigned int) fdclient, &fdsExcept);
	
	if (select(fdclient + 1, &fdsRead, &fdsWrite, &fdsExcept, &nowait) == -1)
		return POLONET_ERROR;
	
	if (FD_ISSET(fdclient, &fdsExcept))
		return POLONET_DISCONNECTED;
	
	if (FD_ISSET(fdclient, &fdsRead))
	{
		unsigned long dataPending;
		
#ifdef WIN32
		if (ioctlsocket(fdclient, FIONREAD, &dataPending) == -1)
			return POLONET_ERROR;
#else
//		if (ioctl(fdclient, FIONREAD, &dataPending) == -1)
//			return POLONET_ERROR;
#endif		
		
		if (dataPending)
			return POLONET_CONNECTED;
		
		return POLONET_DISCONNECTED;
	}
	
	if (!FD_ISSET(fdclient, &fdsWrite))
		return POLONET_PENDING;
	
	return POLONET_CONNECTED;
}

int polonetSend(char *buffer, const int bytesToSend)
{
	if (fdclient == -1)
		return 0;
	
	int status = send(fdclient, buffer, bytesToSend, 0);
	if (status == -1)
	{
		isError = 1;
		return 0;
	}
	
	return status;
}

int polonetReceive(char *buffer, const int bufferSize)
{
	if (fdclient == -1)
		return 0;

	int status = recv(fdclient, buffer, bufferSize, 0);
	if (status == -1)
	{
		isError = 1;
		return 0;
	}
	
	return status;
}

void polonetClose()
{
	polonetCloseSocket(&fdclient);
	polonetCloseSocket(&fdserver);
	
	isError = 0;
}

