#include "stdafx.h"
#include "ServerSocket.h"
#include "TUtil.h"
#include "HighResolutionTime.h"
#include <string.h>
#include <list>

#ifdef _LINUX_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

using namespace std;
//#define FD_SETSIZE 128
//XXX
#define MAX_SOCK_NUM 64	// FD_SETSIZE 64
//#define MAX_SOCK_NUM 4	// FD_SETSIZE 64

namespace TAMENUT {
ServerSocket::ServerSocket(unsigned short bind_port)
	:_user_data_queue(1024 * 1024 * 5)
	, _max_client_cnt(1024)
{
	init(bind_port);
}

ServerSocket::~ServerSocket()
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

void ServerSocket::init(unsigned short bind_port)
{
	bool res_bind = true;
	bool res_listend = true;

	_pkt_size_start_offset = 0;
	_pkt_size_length = 4;

	_connection_flag = false;
	_recv_blocking = false;

	struct sockaddr_in servaddr;
	_sock = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(bind_port);

	int snd_buf_size = SEND_SOCK_BUF_SIZE;
	int rcv_buf_size = RECV_SOCK_BUF_SIZE;
	setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf_size, sizeof(snd_buf_size));
	setsockopt(_sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf_size, sizeof(rcv_buf_size));


	if (bind(_sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
	{
		printf("Error : TCP bind error (port:%u)\n", bind_port);
		res_bind = false;
	}

	if (listen(_sock, _max_client_cnt) == SOCKET_ERROR)
	{
		printf("Error : TCP listen error (port:%u)\n", bind_port);
		res_listend = false;
	}
	_bind_port = bind_port;

	start("TCPSocketSelectThread", THREAD_PRIORITY_TIME_CRITICAL, 8 * 1024 * 1024);


	if (res_bind == true && res_listend == true)
	{
		printf("Success TCP bind and Listend (port:%u)\n", bind_port);
	}

	set_linger();
}
void ServerSocket::set_pkt_len_offset(unsigned short pkt_size_start_offset, unsigned short pkt_size_length)
{
	_pkt_size_start_offset = pkt_size_start_offset;
	_pkt_size_length = pkt_size_length;
}

void ServerSocket::run()
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
			printf("Error : TCP Seclet Error\n");
			break;
		}
		else if (sock_res == 0)
		{
			printf("Error : TCP Select TimeOut\n");
			continue;
		}

		if (FD_ISSET(_sock, &all_socks))
		{
			SOCKET sock = accept(_sock, (struct sockaddr *) &client_addr, &client_len);
			if (sock == SOCKET_ERROR)
			{
				printf("Error : TCP Receive Socket is Closed\n");
				break;
			}
			else if (_client_sock_list.size() >= MAX_SOCK_NUM)
			{
				printf("Error : TCP Accept is Overflow(connection:%d)\n", _client_sock_list.size());
				closesocket(sock);
			}
			else
			{
				ClientSock client_sock;
				client_sock.sock = sock;
				client_sock.addr = client_addr;
				_client_sock_list.push_back(client_sock);
				FD_SET(sock, &read_socks);
				if (sock > max_sock)
					max_sock = sock;

				printf("TCP Accept ====(socket cnt:%d), addr:%s, port:%u \n",
					_client_sock_list.size(), inet_ntoa(client_addr.sin_addr), ntohs(client_sock.addr.sin_port));
			}
		}

		list<ClientSock>::iterator iter;

		//for(unsigned int i=0; i<MAX_SOCK_NUM; i++)
		for (iter = _client_sock_list.begin(); iter != _client_sock_list.end(); )
		{
			int read_ret = 0;
			ClientSock client_sock = *iter;
			if (client_sock.sock != SOCKET_ERROR && FD_ISSET(client_sock.sock, &all_socks))
			{
				read_ret = read_pkt(client_sock.sock, payload, sizeof(payload));
				if (read_ret > 0) {
					push_pkt_queue(payload, read_ret);
				}
			}

			if (read_ret == SOCKET_ERROR)
			{
				printf("Error : TcpSocket rcv_return is -1 (err:%s, %d)\n", strerror(err_num), err_num);
				FD_CLR(client_sock.sock, &read_socks);
				closesocket(client_sock.sock);
				iter = _client_sock_list.erase(iter);
			}
			else if (read_ret == 0)
			{
				printf("Error : TcpSocket rcv_return is 0 (err:%s, %d)\n", strerror(err_num), err_num);
				FD_CLR(client_sock.sock, &read_socks);
				closesocket(client_sock.sock);
				iter = _client_sock_list.erase(iter);
			}
			else //if(read_ret > 0)
			{
				iter++;
			}
		}
	}
}

int ServerSocket::read(char *payload, unsigned int payload_len)
{
	int len = -1;

	do {
		//pop_user_data_queue() 호출시 값이 없을수도 있으므로 반복 수행
		len = pop_user_data_queue(payload, payload_len);
	} while (len == 0);

	return len;
}

int ServerSocket::read(SerializedPayload &serialized_payload)
{
	int len = read(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
	serialized_payload.set_payload_len(len);

	return len;
}

int ServerSocket::read_pkt(SOCKET client_sock, char *payload, unsigned int payload_len)
{
	int pkt_len = 0;
	unsigned int total_pkt_len = 0;
	//패킷길이 4byte만큼 읽기

	pkt_len = recv(client_sock, (char*)&total_pkt_len, sizeof(int), MSG_WAITALL);
	if (total_pkt_len > payload_len)
		pkt_len = -1;

	if (pkt_len > 0) {
		total_pkt_len = ntoh_t(total_pkt_len);
		pkt_len = recv(client_sock, payload, total_pkt_len, MSG_WAITALL);
	}

	return pkt_len;
}

void ServerSocket::push_pkt_queue(char *payload, int payload_len)
{
	if (_recv_blocking == false)
	{
		_queue_lock.lock();
		if (_user_data_queue.push_string(payload, payload_len) == false)
		{
			printf("Error : Can't PushBack to UseDataQueue(queue is full)- In push_user_data_queue()\n");
			printf("Error : ServerSocket Buffer is Full - Packet is droped\n");
		}
		_queue_cond.signal();
		_queue_lock.unlock();
	}
	else
	{
		unsigned int cnt = 0;
		bool res = false;

		do
		{
			_queue_lock.lock();
			res = _user_data_queue.push_string(payload, payload_len);
			_queue_cond.signal();
			_queue_lock.unlock();

			if (res == false)
			{
				msleep(0);
				if (cnt++ / 1000 == 0)
				{
					printf("Error : ServerSocket Buffer is Full - Blocking(block count:%u)\n", cnt);
				}
			}

		} while (res == false);
	}
}

int ServerSocket::pop_user_data_queue(char *payload, unsigned int payload_len)
{
	unsigned int rcv_payload_len;
	_queue_lock.lock();
	if (_user_data_queue.is_empty() == true)
	{
		_queue_cond.wait(&_queue_lock, -1);
	}
	_user_data_queue.pop_string(payload, rcv_payload_len);
	_queue_lock.unlock();

	if (payload_len < rcv_payload_len)
	{
		printf("payload length is not enough - pkt len:%u, reserved pkt len: %u !!\n", rcv_payload_len, payload_len);
	}

	return rcv_payload_len;
}


int ServerSocket::post(SerializedPayload &serialized_payload)
{
	return post(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
}

//tcp 소켓으로 전송하는 함수
int ServerSocket::post(char *payload, unsigned int payload_len)
{
	int sent_len = -1;


	if ((sent_len = _post(payload, payload_len)) == -1)
	{
		if (err_num == 10053 || err_num == 104)
		{
			printf("Error :TCP Socket Post (err:%s)!\n", strerror(err_num));
			_connection_flag = false;
			sent_len = -2;
		}
	}
	
	
	return sent_len;
}

int ServerSocket::_post(char *payload, int payload_len)
{
	int sent_len = 0;
	int sent_result;

	while (sent_len < payload_len)
	{
		sent_result = send(_sock, payload + sent_len, payload_len - sent_len, 0);

		if (sent_result == -1)
		{
			printf("ERROR : tcp send tcp- send_len:%d\n", sent_len);
			sent_len = -1;
			break;
		}
		sent_len += sent_result;
		//	printf("send tcp- send_res:%d, send_len:%d\n", sent_result, sent_len);
	}

	return sent_len;
}

void ServerSocket::set_recv_blocking(bool recv_blocking)
{
	_recv_blocking = recv_blocking;
}

void ServerSocket::set_snd_time_out(int milisec_time_out)
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
		printf("set snd Time out - %d, (res:%d)", res, milisec_time_out);
	}
#endif
}

void ServerSocket::set_rcv_time_out(int milisec_time_out)
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
		//printf("set rcv Time out - %d, (res:%d)\n", res, milisec_time_out);
#endif
	}
	else
	{
		printf("Error : RcvSock is not initialized!\n");
	}
}

bool ServerSocket::is_connection()
{
	return _connection_flag;
}

void ServerSocket::set_linger()
{
	if (_sock != NULL)
	{
		struct linger optval;
		optval.l_onoff = 1;
		optval.l_linger = 0;
		setsockopt(_sock, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));
	}
}

unsigned int ServerSocket::get_current_rcv_buf_size()
{
	return _user_data_queue.get_string_size();
}

unsigned int ServerSocket::get_current_rcv_buf_msg_cnt()
{
	return _user_data_queue.get_string_cnt();
}
}