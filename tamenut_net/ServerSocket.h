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

#include "TStringCircularQueue.h"
#include "TMutex.h"
#include "TCondition.h"

using namespace std;

struct ClientSock
{
	SOCKET sock;
	struct sockaddr_in addr;
};
namespace TAMENUT {
class ServerSocket : public TThread
{
public:
	ServerSocket();
	//Receiver Constructor
	ServerSocket(unsigned short bind_port);
	virtual ~ServerSocket(void);

	int read(char *payload, unsigned int payload_len);
	int read(SerializedPayload &serialized_payload);
	int read_pkt(SOCKET client_sock, char * payload, unsigned int payload_len);
	int post(char *payload, unsigned int length);
	int post(SerializedPayload &serialized_payload);
	void set_recv_blocking(bool recv_blocking);

	void set_snd_time_out(int milisec_time_out);
	void set_rcv_time_out(int milisec_time_out);
	bool is_connection();
	void set_pkt_len_offset(unsigned short pkt_size_start_offset, unsigned short pkt_size_length);
	void set_pkt_len_offest_byte_ordering(bool network_byte_ordering_flag);

	void init(unsigned short bind_port);
	unsigned int get_current_rcv_buf_size();
	unsigned int get_current_rcv_buf_msg_cnt();
protected:

	void run();
	bool connection();
	int read_and_push_user_data_queue(SOCKET sock);
	void push_pkt_queue(char *payload, int payload_len);
	int pop_user_data_queue(char *payload, unsigned int payload_len);
	int _post(char *payload, int payload_len);
	void set_linger();

private:
	SOCKET _sock;
	struct sockaddr_in _servaddr;
	bool _connection_flag;
	unsigned short _bind_port;
	unsigned int _pkt_size_start_offset;
	unsigned int _pkt_size_length;
	unsigned int _max_client_cnt;


	TStringCircularQueue _user_data_queue;
	TMutex _queue_lock;
	TCondition _queue_cond;

	bool _recv_blocking;
	list<ClientSock> _client_sock_list;
};
}

