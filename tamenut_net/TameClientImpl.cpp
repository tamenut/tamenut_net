#include "stdafx.h"
#include "TameClientImpl.h"
#include "TUtil.h"
#include "HighResolutionTime.h"
#include "TameClient.h"
#include <string.h>
#include <list>
#include "SocketDef.h"

#ifdef _LINUX_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

using namespace std;


namespace TAMENUT {
TameClientImpl::TameClientImpl(const char *dst_ip_str, unsigned short bind_port, TameClient * listener)
	:_user_data_queue(1024 * 1024 * 5)
	, _client_listener(listener)
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
	_dst_ip_addr = inet_addr(dst_ip_str);
	_bind_port = bind_port;
}

TameClientImpl::~TameClientImpl()
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

void TameClientImpl::start_client()
{
	init(_dst_ip_addr, _bind_port);
}
void TameClientImpl::init(int dst_ip_addr, unsigned short bind_port)
{
	_connection_flag = false;
	_recv_blocking = false;
	_sock = NULL;

	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		printf("Error : TCP socket error (addr:%x, port:%u)\n", dst_ip_addr, bind_port);
	}

	int snd_buf_size = SEND_SOCK_BUF_SIZE;
	int rcv_buf_size = RECV_SOCK_BUF_SIZE;
	setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf_size, sizeof(snd_buf_size));
	setsockopt(_sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf_size, sizeof(rcv_buf_size));

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
	setsockopt(_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));

}

bool TameClientImpl::connection()
{
	if (_connection_flag == false)
	{
		if (connect(_sock, (struct sockaddr *) &_servaddr, sizeof(_servaddr))<0)
		{
			_connection_flag = false;
		}
		else
		{
			_connection_flag = true;
			if (_client_listener) {
				_client_listener->on_connect();
			}
		}
	}

	return _connection_flag;
}

void TameClientImpl::run()
{
	char payload[MAX_PAYLOAD_SIZE * 8];
	int rcv_len;
	while (is_runnable() == true)
	{
		rcv_len = read_pkt(_sock, payload, sizeof(payload));
		if (rcv_len > 0) {
			push_pkt_queue(payload, rcv_len);
		}
	}
}

int TameClientImpl::read_pkt(SOCKET serv_sock, char *payload, unsigned int payload_len)
{
	int pkt_len = 0;
	unsigned int total_pkt_len = 0;
	//패킷길이 4byte만큼 읽기

	pkt_len = recv(serv_sock, (char*)&total_pkt_len, sizeof(int), MSG_WAITALL);
	if (total_pkt_len > payload_len)
		pkt_len = -1;

	if (pkt_len > 0){
		total_pkt_len = ntoh_t(total_pkt_len);
		pkt_len = recv(serv_sock, payload, total_pkt_len, MSG_WAITALL);
	}

	return pkt_len;
}

int TameClientImpl::read(char *payload, unsigned int payload_len)
{
	int len = -1;

	do {
		//pop_user_data_queue() 호출시 값이 없을수도 있으므로 반복 수행
		len = pop_user_data_queue(payload, payload_len);
	} while (len == 0);

	return len;
}

int TameClientImpl::read(SerializedPayload &serialized_payload)
{
	int len = read(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
	serialized_payload.set_payload_len(len);

	return len;
}


void TameClientImpl::push_pkt_queue(char *payload, int payload_len)
{
	if (_recv_blocking == false)
	{
		_queue_lock.lock();
		if (_user_data_queue.push_string(payload, payload_len) == false)
		{
			printf("Error : Can't PushBack to UseDataQueue(queue is full)- In push_user_data_queue()\n");
			printf("Error : TameClientImpl Buffer is Full - Packet is droped\n");
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
					printf("Error : TameClientImpl Buffer is Full - Blocking(block count:%u)\n", cnt);
				}
			}

		} while (res == false);
	}
}

int TameClientImpl::pop_user_data_queue(char *payload, unsigned int payload_len)
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

int TameClientImpl::post(SerializedPayload &serialized_payload)
{
	return post(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
}

//tcp 소켓으로 전송하는 함수
int TameClientImpl::post(char *payload, unsigned int payload_len)
{
	int sent_len = -1;

	if (connection() == true)
	{
		unsigned int ordered_payload_len = hton_t(payload_len);
		if ((sent_len = _post((char*)&ordered_payload_len, sizeof(ordered_payload_len))) == -1)
		{
		}

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


	return sent_len;
}

int TameClientImpl::_post(char *payload, int payload_len)
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

void TameClientImpl::set_recv_blocking(bool recv_blocking)
{
	_recv_blocking = recv_blocking;
}

void TameClientImpl::set_snd_time_out(int milisec_time_out)
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

void TameClientImpl::set_rcv_time_out(int milisec_time_out)
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
		int res = setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_val, opt_len);
		//printf("set rcv Time out - %d, (res:%d)\n", res, milisec_time_out);
#endif
	}
	else
	{
		printf("Error : RcvSock is not initialized!\n");
	}
}

bool TameClientImpl::is_connection()
{
	return _connection_flag;
}

void TameClientImpl::set_linger()
{
	if (_sock != NULL)
	{
		struct linger optval;
		optval.l_onoff = 1;
		optval.l_linger = 0;
		setsockopt(_sock, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));
	}
}

unsigned int TameClientImpl::get_current_rcv_buf_size()
{
	return _user_data_queue.get_string_size();
}

unsigned int TameClientImpl::get_current_rcv_buf_msg_cnt()
{
	return _user_data_queue.get_string_cnt();
}

void TameClientImpl::set_listener(TameClient * listener)
{
	_client_listener = listener;
}

}