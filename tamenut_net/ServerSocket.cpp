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

ServerSocket::ServerSocket()
{
}


ServerSocket::~ServerSocket()
{
}


using namespace std;
//#define FD_SETSIZE 128
//XXX
#define MAX_SOCK_NUM 64	// FD_SETSIZE 64
//#define MAX_SOCK_NUM 4	// FD_SETSIZE 64


ServerSocket::ServerSocket()
	:_user_data_queue(1024 * 1024 * 5)
{

}

//수신용 socket (server)
ServerSocket::ServerSocket(unsigned short bind_port)
	:_user_data_queue(1024 * 1024 * 5)
{
	receiver_init(bind_port);
}

//송신용 socket (client)
ServerSocket::ServerSocket(const char *dst_ip_str, unsigned short bind_port)
{
	sender_init(inet_addr(dst_ip_str), bind_port);
}

ServerSocket::~ServerSocket()
{
	//패킷 수신을 위해 소켓을 오픈하고 있으므로 쓰레드 종료가 소켓종료보다 먼저 실행되어야함
	//stop();
#ifdef WIN32
	if (_rcv_sock != NULL)
	{
		closesocket(_rcv_sock);
	}

	if (_snd_sock != NULL)
	{
		closesocket(_snd_sock);
	}
#else
	if (_rcv_sock != 0)
	{
		shutdown(_rcv_sock, SHUT_RDWR);
		close(_rcv_sock);
	}

	if (_snd_sock != 0)
	{
		shutdown(_snd_sock, SHUT_RDWR);
		close(_snd_sock);
	}
#endif
}

void ServerSocket::sender_init(int dst_ip_addr, unsigned short bind_port)
{
	_socket_type = SEND_SOCKET_TYPE;
	_pkt_size_start_offset = 0;
	_pkt_size_length = 4;
	_is_network_byte_ordering = true;

	_connection_flag = false;
	_recv_blocking = false;
	_rcv_sock = NULL;

	if ((_snd_sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		printf("Error : TCP socket error (addr:%x, port:%u)\n", dst_ip_addr, bind_port);
	}

	int snd_buf_size = SEND_SOCK_BUF_SIZE;
	setsockopt(_snd_sock, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf_size, sizeof(snd_buf_size));

	//Creation of the socket
	memset(&_servaddr, 0, sizeof(_servaddr));
	_servaddr.sin_family = AF_INET;

	_servaddr.sin_addr.s_addr = dst_ip_addr;
	_servaddr.sin_port = htons(bind_port);

	if (connection() == false)
	{
		printf("Error : TCP Socket Connect error(addr:%x, port:%u)\n", dst_ip_addr, bind_port);
	}

	int opt_val = 1;
	setsockopt(_snd_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));

}

void ServerSocket::receiver_init(unsigned short bind_port)
{
	bool res_bind = true;
	bool res_listend = true;

	_socket_type = RECEIVE_SOCKET_TYPE;
	_pkt_size_start_offset = 0;
	_pkt_size_length = 4;
	_is_network_byte_ordering = true;

	_connection_flag = false;
	_recv_blocking = false;

	struct sockaddr_in servaddr;
	_snd_sock = NULL;
	_rcv_sock = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(bind_port);

	int rcv_buf_size = RECEIVE_SOCK_BUF_SIZE;
	setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf_size, sizeof(rcv_buf_size));


	if (bind(_rcv_sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
	{
		printf("Error : TCP bind error (port:%u)\n", bind_port);
		res_bind = false;
	}

	if (listen(_rcv_sock, 12) == SOCKET_ERROR)
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

void ServerSocket::set_pkt_len_offest_byte_ordering(bool network_byte_ordering_flag)
{
	_is_network_byte_ordering = network_byte_ordering_flag;
}

bool ServerSocket::connection()
{
	if (_connection_flag == false)
	{
		if (connect(_snd_sock, (struct sockaddr *) &_servaddr, sizeof(_servaddr))<0)
		{
			_connection_flag = false;
			//printf("Error : connection fail!!\n");
		}
		else
		{
			_connection_flag = true;
			//printf("connection success!!\n");
		}
	}

	return _connection_flag;
}

void ServerSocket::run()
{
	struct timeval timeout;
	int sock_res;
	fd_set read_socks, all_socks;

	SOCKET max_sock;
	struct sockaddr_in client_addr;
#ifdef WIN32
	int client_len = sizeof(client_addr);
#else
	socklen_t  client_len = sizeof(client_addr);
#endif

	FD_ZERO(&read_socks);
	FD_SET(_rcv_sock, &read_socks);

	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	max_sock = _rcv_sock;

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

		if (FD_ISSET(_rcv_sock, &all_socks))
		{
			SOCKET sock = accept(_rcv_sock, (struct sockaddr *) &client_addr, &client_len);
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
			int read_ret = 1;
			ClientSock client_sock = *iter;
			if (client_sock.sock != SOCKET_ERROR && FD_ISSET(client_sock.sock, &all_socks))
			{
				read_ret = read_and_push_user_data_queue(client_sock.sock);
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
	if (_socket_type == RECEIVE_SOCKET_TYPE)
	{
		do {
			//pop_user_data_queue() 호출시 값이 없을수도 있으므로 반복 수행
			len = pop_user_data_queue(payload, payload_len);
		} while (len == 0);
	}
	else
	{
		printf("Error : TcpSocket's type is SendSocket!!\n");
	}
	return len;
}

int ServerSocket::read(SerializedPayload &serialized_payload)
{
	int len = read(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
	serialized_payload.set_payload_len(len);

	return len;
}

int ServerSocket::read_and_push_user_data_queue(SOCKET client_sock)
{
	int pkt_len = 0;
	char pkt_len_payload[MAX_PAYLOAD_SIZE];
	char payload[MAX_PAYLOAD_SIZE * 8];
	unsigned int total_pkt_len = 0;
	unsigned int hdr_length;
	//패킷길이 4byte만큼 읽기
#if 0
	pkt_len = recv(client_sock, (char*)&total_pkt_len, sizeof(int), MSG_WAITALL);
#else
	hdr_length = _pkt_size_start_offset + _pkt_size_length;
	pkt_len = recv(client_sock, (char*)&pkt_len_payload, hdr_length, MSG_WAITALL);

	if (_is_network_byte_ordering)
	{
		ntohl_t(pkt_len_payload + _pkt_size_start_offset, (char*)&total_pkt_len);
	}
	else
	{
		memcpy(&total_pkt_len, pkt_len_payload + _pkt_size_start_offset, _pkt_size_length);
	}

#endif

	if (pkt_len > 0 &&
		total_pkt_len - hdr_length >= 0 &&
		sizeof(payload) > total_pkt_len - hdr_length)
	{
		memcpy(payload, pkt_len_payload, hdr_length);

		pkt_len = recv(client_sock, payload + hdr_length, total_pkt_len - hdr_length, MSG_WAITALL);

		//if(pkt_len != SOCKET_ERROR && pkt_len != 0)
		if (pkt_len > 0 && pkt_len <= sizeof(payload))
		{
			//push_user_data_queue(payload, pkt_len);
			push_user_data_queue(payload, total_pkt_len);
		}
		else
		{
			//DeleteMemory(payload);
			printf("Error : Received TCP Packet size : %d (err_code:%d)\n", pkt_len, err_num);
		}
	}
	else
	{
		printf("Error : Received TCP Packet pkt_len: %d, total_pkt_len:%u, pkt_size_offset:%u, pkt_size_len:%u, bind_port:%u (err_code:%d)\n",
			pkt_len, total_pkt_len, _pkt_size_start_offset, _pkt_size_length, _bind_port, err_num);
		for (unsigned int i = 0; i < hdr_length; i++)
		{
			printf("%x ", pkt_len_payload[i]);
		}
		printf("\n");
	}

	return pkt_len;
}

void ServerSocket::push_user_data_queue(char *payload, int payload_len)
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
	if (_socket_type == SEND_SOCKET_TYPE)
	{
		if (connection() == true)
		{
			if ((sent_len = _post(payload, payload_len)) == -1)
			{
				if (err_num == 10053 || err_num == 104)
				{
					printf("Error :TCP Socket Post (err:%s)!\n", strerror(err_num));
					_connection_flag = false;
					sent_len = -2;
				}
			}
		}
	}
	else
	{
		printf("Error : TcpSocket's type is ReceiveSocket!!\n");
	}

	return sent_len;
}

int ServerSocket::_post(char *payload, int payload_len)
{
	int sent_len = 0;
	int sent_result;

	while (sent_len < payload_len)
	{

#if 0
		HighResolutionTime tm;
		tm.start();
		sent_result = send(_snd_sock, payload + sent_len, payload_len - sent_len, 0);
		long long elaps = tm.get_elapsed_micro_sec();
		if (elaps > 100)
		{
			printf("post elapse time : %lld \n", elaps);
		}

#else
		sent_result = send(_snd_sock, payload + sent_len, payload_len - sent_len, 0);
#endif
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
	if (_snd_sock != NULL)
	{
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_snd_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&milisec_time_out, opt_len);
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
	if (_rcv_sock)
	{
#ifdef WIN32
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&milisec_time_out, opt_len);
#else
		struct timeval opt_val;
		opt_val.tv_sec = milisec_time_out / 1000;
		opt_val.tv_usec = (milisec_time_out % 1000) * 1000;
		int opt_len = sizeof(opt_val);
		int res = setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_val, opt_len);
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
	if (_rcv_sock != NULL)
	{
		struct linger optval;
		optval.l_onoff = 1;
		optval.l_linger = 0;
		setsockopt(_rcv_sock, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));
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