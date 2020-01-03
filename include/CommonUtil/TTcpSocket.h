#pragma once

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#endif
#include "TThread.h"

#include "TSocket.h"
#include <string.h>
#include "SerializedPayload.h"
#include <list>

#define USE_SERIALIZED_QUEUE

#ifdef USE_SERIALIZED_QUEUE
#include "TStringCircularQueue.h"
#include "TMutex.h"
#include "TCondition.h"
#else
#include "TMsgQueue.h"
#endif

using namespace std;

struct ClientSock
{
	SOCKET sock;
	struct sockaddr_in addr;
};

class TTcpSocket : public TThread, public TSocket
{
public:
	UTIL_DLLFunction TTcpSocket();
	//Receiver Constructor
	UTIL_DLLFunction TTcpSocket(unsigned short bind_port);

	//Sender Constructor
	UTIL_DLLFunction TTcpSocket(const char *dst_ip_str, unsigned short bind_port);


	UTIL_DLLFunction virtual ~TTcpSocket(void);

	UTIL_DLLFunction int read(char *payload, unsigned int payload_len);
	UTIL_DLLFunction int read(SerializedPayload &serialized_payload);
	UTIL_DLLFunction int post(char *payload, unsigned int length);
	UTIL_DLLFunction int post(SerializedPayload &serialized_payload);
	UTIL_DLLFunction void set_recv_blocking(bool recv_blocking);

	UTIL_DLLFunction void set_snd_time_out(int milisec_time_out);
	UTIL_DLLFunction void set_rcv_time_out(int milisec_time_out);
	UTIL_DLLFunction bool is_connection();
	UTIL_DLLFunction void set_pkt_len_offset(unsigned short pkt_size_start_offset, unsigned short pkt_size_length);
	UTIL_DLLFunction void set_pkt_len_offest_byte_ordering(bool network_byte_ordering_flag);

	UTIL_DLLFunction void sender_init(int dst_ip_addr, unsigned short bind_port);
	UTIL_DLLFunction void receiver_init(unsigned short bind_port);
	UTIL_DLLFunction unsigned int get_current_rcv_buf_size();
	UTIL_DLLFunction unsigned int get_current_rcv_buf_msg_cnt();
protected:

	void run();
	bool connection();
	int read_and_push_user_data_queue(SOCKET sock);
	void push_user_data_queue(char *payload, int payload_len);
	int pop_user_data_queue(char *payload, unsigned int payload_len);
	int _post(char *payload, int payload_len);
	void set_linger();

private:
	SOCKET _snd_sock;
	struct sockaddr_in _servaddr;
	bool _connection_flag;
	unsigned short _bind_port;
	unsigned int _pkt_size_start_offset;
	unsigned int _pkt_size_length;

#ifdef USE_SERIALIZED_QUEUE
	TStringCircularQueue _user_data_queue;
	TMutex _queue_lock;
	TCondition _queue_cond;
#else
	TMsgQueue<QueueNode> _user_data_queue;
#endif
	bool _recv_blocking;
	bool _is_network_byte_ordering;
	list<ClientSock> _client_sock_list;
};


