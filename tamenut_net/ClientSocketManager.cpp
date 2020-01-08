#include "stdafx.h"
#include "ClientSocketManager.h"
#include "LogModule.h"

ClientSocketManager::ClientSocketManager(unsigned int max_socket_cnt)
	:_max_client_cnt(max_socket_cnt)
	, _current_client_cnt(0)
{
	for (unsigned int i = 0; i < _max_client_cnt; i++) {
		_client_sock_list.push_back(ClientSock());
		_client_id_list.push_back(false);
	}
}


ClientSocketManager::~ClientSocketManager()
{
}

unsigned int ClientSocketManager::get_max_client_cnt()
{
	return _max_client_cnt;
}

unsigned int ClientSocketManager::get_current_client_cnt()
{
	return _current_client_cnt;
}

unsigned int ClientSocketManager::add_client_sock(SOCKET sock, struct sockaddr_in addr)
{
	ClientSock client_sock;
	client_sock._sock = sock;
	client_sock._addr = addr;
	return add_client_sock(client_sock);
}

unsigned int ClientSocketManager::add_client_sock(ClientSock c_sock)
{
	unsigned int client_id = 0;
	for (unsigned int i = 0; i < _client_sock_list.size(); i++) {
		if (_client_sock_list[i]._sock == SOCKET_ERROR) {
			_client_sock_list[i] = c_sock;
			client_id = i + 1;
			_current_client_cnt++;
			break;

		}
	}
	return client_id;
}
ClientSock ClientSocketManager::get_client_sock(unsigned int client_id)
{
	ClientSock res;
	unsigned int client_idx = client_id - 1;

	if (client_idx >= 0 && client_idx < _client_sock_list.size()) {
		res = _client_sock_list[client_idx];
	}
	else {
		ERROR_LOG("client_id:%u - (%s)!!\n", __FUNCTION__);
	}
	return res;
}

ClientSock ClientSocketManager::get_client_sock(fd_set sock_fd_set)
{
	ClientSock res;
	for (unsigned int i = 0; i < _client_sock_list.size(); i++) {
		if (_client_sock_list[i]._sock != SOCKET_ERROR && 
			FD_ISSET(_client_sock_list[i]._sock, &sock_fd_set)) {
			res = _client_sock_list[i];
		}
	}
	return res;
}

bool ClientSocketManager::delete_client_sock(ClientSock c_sock)
{
	bool res = false;
	unsigned int client_idx = c_sock._client_id - 1;
	closesocket(c_sock._sock);
	if (client_idx >= 0 && client_idx < _client_sock_list.size()) {
		_client_sock_list[client_idx].init();
		_current_client_cnt--;
		res = true;
	}
	else {
		ERROR_LOG("Can't delet client sock(c_sock._client_id:%u) (%s)\n", 
			c_sock._client_id, __FUNCTION__);
		res = false;
	}
	return res;
}