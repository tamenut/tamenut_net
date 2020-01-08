#pragma once
#include "ClientSocketManager.h"
#include "TThread.h"

#include "TSocket.h"
#include <string.h>
#include "SerializedPayload.h"
#include <vector>

#include "TStringCircularQueue.h"
#include "TMutex.h"
#include "TCondition.h"

using namespace std;

namespace TAMENUT {
class TameServer;

class TameServerImpl : public TThread
{
private:
	SOCKET _sock;
	struct sockaddr_in _servaddr;
	bool _connection_flag;
	unsigned short _bind_port;
	unsigned int _pkt_size_start_offset;
	unsigned int _pkt_size_length;
	
	TameServer *_server_listener;

	TStringCircularQueue _user_data_queue;
	TMutex _queue_lock;
	TCondition _queue_cond;

	bool _recv_blocking;
	ClientSocketManager _client_socket_manger;

public:
	TameServerImpl(unsigned short bind_port, TameServer * listener = NULL);
	virtual ~TameServerImpl(void);

	void start_server();

	int read(char *payload, unsigned int payload_len);
	int read(SerializedPayload &serialized_payload);
	int read_pkt(SOCKET client_sock, char * payload, unsigned int payload_len);
	int post(unsigned int cid, SerializedPayload &serialized_payload);
	int post(unsigned int cid, char * payload, unsigned int payload_len);
	int _post(SOCKET sock, char * payload, unsigned int payload_len);
	void set_recv_blocking(bool recv_blocking);

	void set_snd_time_out(int milisec_time_out);
	void set_rcv_time_out(int milisec_time_out);
	bool is_connection();
	unsigned short get_bind_port();
	void set_pkt_len_offset(unsigned short pkt_size_start_offset, unsigned short pkt_size_length);

	void init(unsigned short bind_port);
	unsigned int get_current_rcv_buf_size();
	unsigned int get_current_rcv_buf_msg_cnt();
	void set_listener(TameServer *listener);
		

protected:

	void run();
	void push_pkt_queue(char *payload, int payload_len);
	int pop_user_data_queue(char *payload, unsigned int payload_len);
	void set_linger();
};
}

