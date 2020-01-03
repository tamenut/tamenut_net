#pragma once
#include "CommonDllDefine.h"
#include "TUtil.h"

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

#define SEND_SOCK_BUF_SIZE 1*1024*1024
#define RECEIVE_SOCK_BUF_SIZE 1*1024*1024

struct QueueNode
{
	int payload_len;
	char *payload;
};

enum SOCKET_TYPE{
	NONE_SOCKET_TYPE = 0,
	SEND_SOCKET_TYPE = 1,
	RECEIVE_SOCKET_TYPE = 2
};

class TSocket
{
public:
	TSocket(void);
	virtual ~TSocket(void);
	UTIL_DLLFunction virtual void sender_init(int dst_ip_addr, unsigned short bind_port) = 0;
	UTIL_DLLFunction virtual void receiver_init(unsigned short bind_port) = 0;

	UTIL_DLLFunction virtual int post(char *payload, unsigned int length) = 0;
	UTIL_DLLFunction virtual int read(char *payload, unsigned int rcv_len) = 0;

protected:
	SOCKET_TYPE _socket_type;
	SOCKET _rcv_sock;

};


