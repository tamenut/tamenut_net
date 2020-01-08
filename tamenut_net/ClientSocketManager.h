#pragma once
#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#endif
#include <vector>

struct ClientSock
{
	SOCKET _sock;
	unsigned int _client_id;
	struct sockaddr_in _addr;
	ClientSock()
	{
		init();
	}
	void init()
	{
		_sock = SOCKET_ERROR;
		_client_id = 0;
		_addr.sin_addr.S_un.S_addr = 0;
	}
};

class ClientSocketManager
{
private:
	unsigned int _max_client_cnt;
	unsigned int _current_client_cnt;
	std::vector<ClientSock> _client_sock_list;
	std::vector<bool> _client_id_list;

public:
	ClientSocketManager(unsigned int max_socket_cnt);
	~ClientSocketManager();
	unsigned int get_max_client_cnt();
	unsigned int get_current_client_cnt();
	unsigned int add_client_sock(SOCKET sock, sockaddr_in addr);
	ClientSock get_client_sock(fd_set sock_fd_set);
	bool delete_client_sock(ClientSock c_sock);

private:
	unsigned int add_client_sock(ClientSock c_sock);
};

