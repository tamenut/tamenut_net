#pragma once
#include "TThread.h"
#include "TStringCircularQueue.h"
#include "TMutex.h"
#include "TCondition.h"
#include "SerializedPayload.h"

namespace TAMENUT {
class TameServer;

class ProcPktThread : public TThread
{
private:
	TStringCircularQueue _user_data_queue;
	TMutex _queue_lock;
	TCondition _queue_cond;
	bool _recv_blocking;
	TameServer *_server_listener;

	
public:
	ProcPktThread(TameServer * listener);
	~ProcPktThread();
	void set_listener(TameServer * listener);
	virtual void run();
	
	void push_pkt_queue(char * payload, int payload_len);
	int pop_user_data_queue(char * payload, unsigned int payload_len);

	int read(char *payload, unsigned int payload_len);
	int read(SerializedPayload &serialized_payload);
	

private:

};
}

