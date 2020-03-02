#include "stdafx.h"
#include "ProcPktThread.h"
#include "TameServer.h"
#include "TUtil.h"
#include "LogModule.h"

namespace TAMENUT {
ProcPktThread::ProcPktThread(TameServer * listener)
	:_server_listener(listener)
	, _user_data_queue(1024 * 5)
{
	//_recv_blocking = false;
	
}


ProcPktThread::~ProcPktThread()
{
}

void ProcPktThread::set_listener(TameServer * listener)
{
	//_server_listener = listener;
}

void ProcPktThread::run()
{
	char payload[1024*1024];
	int rcv_len = 0;
	unsigned int client_id;;
	while (is_runnable() == true)
	{
		rcv_len = pop_user_data_queue((char*)&client_id, sizeof(client_id));
		rcv_len = pop_user_data_queue(payload, sizeof(payload));
		if (_server_listener) {
			_server_listener->on_message(client_id, (TAMENUT::SerializedPkt *) payload);
		}
		msleep(1000);
	}
}

void ProcPktThread::push_pkt_queue(char *payload, int payload_len)
{
	if (_recv_blocking == false)
	{
		_queue_lock.lock();
		if (_user_data_queue.push_string(payload, payload_len) == false)
		{
			ERROR_LOG("Can't PushBack to UseDataQueue(queue is full)- In push_user_data_queue()\n");
			ERROR_LOG("TameServerImpl Buffer is Full - Packet is droped\n");
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
					printf("Error : TameServerImpl Buffer is Full - Blocking(block count:%u)\n", cnt);
				}
			}

		} while (res == false);
	}
}

int ProcPktThread::pop_user_data_queue(char *payload, unsigned int payload_len)
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

int ProcPktThread::read(char *payload, unsigned int payload_len)
{
	int len = -1;

	do {
		//pop_user_data_queue() 호출시 값이 없을수도 있으므로 반복 수행
		len = pop_user_data_queue(payload, payload_len);
	} while (len == 0);

	return len;
}

int ProcPktThread::read(SerializedPayload &serialized_payload)
{
	int len = read(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
	serialized_payload.set_payload_len(len);

	return len;
}

}
