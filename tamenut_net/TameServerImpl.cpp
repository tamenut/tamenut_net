#include "stdafx.h"
#include "TameServerImpl.h"
#include "TUtil.h"
#include "HighResolutionTime.h"
#include "TameServer.h"
#include <string.h>
#include <list>
#include "SocketDef.h"
#include "LogModule.h"
#include "ProcPktThread.h"

#ifdef _LINUX_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

using namespace std;

#define MAX_SOCK_NUM 1024

namespace TAMENUT {
TameServerImpl::TameServerImpl(unsigned short bind_port, TameServer * listener)
	:_server_listener(listener)
	, _client_socket_manger(MAX_SOCK_NUM)
{
#if defined(WIN32)
	static bool is_process = false;
	if (is_process == false)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		is_process = true;
	}
#endif
	for (unsigned int i = 0; i < 1; i++) {
		ProcPktThread *tmp = new ProcPktThread(listener);
		_proc_pkt_thread_list.push_back(tmp);
	}
	
	_bind_port = bind_port;
}

TameServerImpl::~TameServerImpl()
{
	//패킷 수신을 위해 소켓을 오픈하고 있으므로 쓰레드 종료가 소켓종료보다 먼저 실행되어야함
	//stop();
#ifdef WIN32
	if (_sock != NULL)
	{
		closesocket(_sock);
	}

#else
	if (_sock != 0)
	{
		shutdown(_sock, SHUT_RDWR);
		close(_sock);
	}
#endif
}
void TameServerImpl::start_server()
{
	for (unsigned int i = 0; i < _proc_pkt_thread_list.size(); i++) {
		_proc_pkt_thread_list[i]->start("ProcPktThread", THREAD_PRIORITY_NORMAL, 1024 * 1024 * 4);
	}
	init(_bind_port);
}
void TameServerImpl::init(unsigned short bind_port)
{
	bool res_bind = true;
	bool res_listend = true;
	_pkt_size_start_offset = 0;
	_pkt_size_length = 4;

	_connection_flag = false;

	struct sockaddr_in servaddr;
	_sock = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(bind_port);
	/*
	int snd_buf_size = SEND_SOCK_BUF_SIZE;
	int rcv_buf_size = RECV_SOCK_BUF_SIZE;
	setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf_size, sizeof(snd_buf_size));
	setsockopt(_sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf_size, sizeof(rcv_buf_size));
	*/

	if (bind(_sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
	{
		ERROR_LOG("TCP bind error (port:%u, err:%s)\n", bind_port, strerror(err_num));
		res_bind = false;
	}

	if (listen(_sock, _client_socket_manger.get_max_client_cnt()) == SOCKET_ERROR)
	{
		ERROR_LOG("TCP listen error (port:%u, err:%s)\n", bind_port, strerror(err_num));
		res_listend = false;
	}
	_bind_port = bind_port;

	start("TCPSocketSelectThread", THREAD_PRIORITY_TIME_CRITICAL, 8 * 1024 * 1024);


	if (res_bind == true && res_listend == true)
	{
		ERROR_LOG("Success TCP bind and Listend (port:%u)\n", bind_port);
	}

	set_linger();
}

unsigned short TameServerImpl::get_bind_port()
{
	return _bind_port;
}

void TameServerImpl::set_pkt_len_offset(unsigned short pkt_size_start_offset, unsigned short pkt_size_length)
{
	_pkt_size_start_offset = pkt_size_start_offset;
	_pkt_size_length = pkt_size_length;
}

void TameServerImpl::run()
{
	struct timeval timeout;
	int sock_res;
	fd_set read_socks, all_socks;
	char payload[MAX_PAYLOAD_SIZE * 8];

	SOCKET max_sock;
	struct sockaddr_in client_addr;
#ifdef WIN32
	int client_len = sizeof(client_addr);
#else
	socklen_t  client_len = sizeof(client_addr);
#endif

	FD_ZERO(&read_socks);
	FD_SET(_sock, &read_socks);

	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	max_sock = _sock;

	while (is_runnable() == true)
	{
		all_socks = read_socks;
		//sock_res = select(max_sock+1, &all_socks, (fd_set*)0, (fd_set*)0, &timeout);
		sock_res = select(max_sock + 1, &all_socks, (fd_set*)0, (fd_set*)0, NULL);

		if (sock_res == SOCKET_ERROR)
		{
			ERROR_LOG("TCP Seclet Error\n");
			break;
		}
		else if (sock_res == 0)
		{
			ERROR_LOG("TCP Select TimeOut\n");
			continue;
		}

		if (FD_ISSET(_sock, &all_socks))
		{
			SOCKET sock = accept(_sock, (struct sockaddr *) &client_addr, &client_len);
			if (sock == SOCKET_ERROR)
			{
				ERROR_LOG("TCP Receive Socket is Closed\n");
				break;
			}
			else if (_client_socket_manger.get_current_client_cnt() >= MAX_SOCK_NUM)
			{
				ERROR_LOG("TCP Accept is Overflow(connection:%d)\n",
					_client_socket_manger.get_current_client_cnt());
				closesocket(sock);
			}
			else
			{
				unsigned int client_id = _client_socket_manger.add_client_sock(sock, client_addr);
				if (client_id >= 0) {
					FD_SET(sock, &read_socks);

					if (sock > max_sock)
						max_sock = sock;

					ERROR_LOG("TCP Accept ====(socket cnt:%d), addr:%s, port:%u \n",
						_client_socket_manger.get_current_client_cnt(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					if (_server_listener) {
						_server_listener->on_connect(client_id);
					}
				}
			}
		}

		int read_ret = 1;
		ClientSock client_sock = _client_socket_manger.get_client_sock(all_socks);

		if (client_sock._sock != SOCKET_ERROR && FD_ISSET(client_sock._sock, &all_socks))
		{
			read_ret = read_pkt(client_sock._sock, payload, sizeof(payload));
			if (read_ret > 0) {
				push_pkt_queue(client_sock, payload, read_ret);
			}
		}

		if (read_ret == SOCKET_ERROR)
		{
			ERROR_LOG("TcpSocket rcv_return is -1 (err:%s, %d)\n", strerror(err_num), err_num);
			FD_CLR(client_sock._sock, &read_socks);
			_client_socket_manger.delete_client_sock(client_sock);
		}
		else if (read_ret == 0)
		{
			ERROR_LOG("TcpSocket rcv_return is 0 (err:%s, %d)\n", strerror(err_num), err_num);
			FD_CLR(client_sock._sock, &read_socks);
			
			_client_socket_manger.delete_client_sock(client_sock);
		}
	}
	
}



int TameServerImpl::read_pkt(SOCKET client_sock, char *payload, unsigned int payload_len)
{
	int pkt_len = 0;
	unsigned int total_pkt_len = 0;
	//패킷길이 4byte만큼 읽기

	pkt_len = recv(client_sock, (char*)&total_pkt_len, sizeof(int), MSG_WAITALL);
	total_pkt_len = ntoh_t(total_pkt_len);
	if (total_pkt_len > payload_len)
		pkt_len = -1;

	if (pkt_len > 0) {		
		pkt_len = recv(client_sock, payload, total_pkt_len, MSG_WAITALL);
	}

	return pkt_len;
}

void TameServerImpl::push_pkt_queue(ClientSock client_sock, char *payload, int payload_len)
{
	_proc_pkt_thread_list[0]->push_pkt_queue((char*)&client_sock._client_id, sizeof(client_sock._client_id));
	_proc_pkt_thread_list[0]->push_pkt_queue(payload, payload_len);
}

int TameServerImpl::post(unsigned int client_id, SerializedPayload &serialized_payload)
{
	return _post(client_id, serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
}

int TameServerImpl::post(unsigned int client_id, char *payload, unsigned int payload_len)
{
	ClientSock sock = _client_socket_manger.get_client_sock(client_id);
	return _post(sock._sock, payload, payload_len);
}

//tcp 소켓으로 전송하는 함수
int TameServerImpl::_post(SOCKET sock, char *payload, unsigned int payload_len)
{
	unsigned int sent_len = 0;
	int sent_result;

	while (sent_len < payload_len)
	{
		sent_result = send(_sock, payload + sent_len, payload_len - sent_len, MSG_WAITALL);

		if (sent_result == -1)
		{
			ERROR_LOG("ERROR : tcp send tcp- send_len:%d, err:%s (%s)\n", 
				sent_len, strerror(err_num), __FUNCTION__);
			sent_len = -1;
			break;
		}
		sent_len += sent_result;
		//	ERROR_LOG("send tcp- send_res:%d, send_len:%d\n", sent_result, sent_len);
	}

	return sent_len;
}

void TameServerImpl::set_snd_time_out(int milisec_time_out)
{
#ifdef WIN32
	if (_sock != NULL)
	{
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&milisec_time_out, opt_len);
	}
#else
	if (_snd_sock != 0)
	{
		struct timeval opt_val;
		opt_val.tv_sec = milisec_time_out / 1000;
		opt_val.tv_usec = (milisec_time_out % 1000) * 1000;
		int opt_len = sizeof(opt_val);
		int res = setsockopt(_snd_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&opt_val, opt_len);
		ERROR_LOG("set snd Time out - %d, (res:%d)", res, milisec_time_out);
	}
#endif
}

void TameServerImpl::set_rcv_time_out(int milisec_time_out)
{
	if (_sock)
	{
#ifdef WIN32
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&milisec_time_out, opt_len);
#else
		struct timeval opt_val;
		opt_val.tv_sec = milisec_time_out / 1000;
		opt_val.tv_usec = (milisec_time_out % 1000) * 1000;
		int opt_len = sizeof(opt_val);
		int res = setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_val, opt_len);
		//ERROR_LOG("set rcv Time out - %d, (res:%d)\n", res, milisec_time_out);
#endif
	}
	else
	{
		ERROR_LOG("Error : RcvSock is not initialized!\n");
	}
}

bool TameServerImpl::is_connection()
{
	return _connection_flag;
}

void TameServerImpl::set_linger()
{
	if (_sock != NULL)
	{
		struct linger optval;
		optval.l_onoff = 1;
		optval.l_linger = 0;
		setsockopt(_sock, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));
	}
}

void TameServerImpl::set_listener(TameServer * listener)
{
	_server_listener = listener;
}


}