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

namespace TAMENUT {
class TameClient;

class TameClientImpl : public TThread
{
public:
	//Sender Constructor
	TameClientImpl(const char *dst_ip_str, unsigned short bind_port, TameClient * listener = NULL);

	virtual ~TameClientImpl(void);

	int read(char *payload, unsigned int payload_len);
	int read(SerializedPayload &serialized_payload);
	int post(char *payload, unsigned int length);
	int post(SerializedPayload &serialized_payload);
	void set_recv_blocking(bool recv_blocking);

	void set_snd_time_out(int milisec_time_out);
	void set_rcv_time_out(int milisec_time_out);
	bool is_connection();

	void start_client();

	void init(int dst_ip_addr, unsigned short bind_port);

	unsigned int get_current_rcv_buf_size();
	unsigned int get_current_rcv_buf_msg_cnt();
	void set_listener(TameClient *listener);

protected:

	void run();
	int read_pkt(SOCKET serv_sock, char * payload, unsigned int payload_len);
	bool connection();
	void push_pkt_queue(char *payload, int payload_len);
	int pop_user_data_queue(char *payload, unsigned int payload_len);
	int _post(char *payload, int payload_len);
	void set_linger();

private:
	SOCKET _sock;
	bool _connection_flag;
	int _dst_ip_addr;
	unsigned short _bind_port;
	struct sockaddr_in _servaddr;
	TameClient *_client_listener;

	TStringCircularQueue _user_data_queue;
	TMutex _queue_lock;
	TCondition _queue_cond;

	bool _recv_blocking;
};
}

