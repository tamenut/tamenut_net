#pragma once

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>

#define SOCKET_ERROR	-1
#define SOCKET			int
#define closesocket		close
#endif

class SocketListener {

};

