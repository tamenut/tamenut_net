#pragma once

#include <vector>
#include <list>
#include "CommonDllDefine.h"
#include "TCircularQueue.h"
#include "TMutex.h"
#include "TCondition.h"

//#include "TCond.h"

//#include "SharedMemory.h"

#define MAX_MSG_QUEUE_NAME_SIZE 256
#define MSG_QUEUE_MAX_SIZE 1024

using namespace std;
template<class T>
class TMsgQueue
{
public:
	TMsgQueue(int size = MSG_QUEUE_MAX_SIZE)
		:_queue(size)
	{
		_queue_lock = new TMutex();
		_queue_cond = new TCondition();
	}
	~TMsgQueue(void)
	{
		delete _queue_lock;
		delete _queue_cond;
	}

	void resize(int size)
	{
		_queue.resize(size);
	}

	unsigned int get_msg_cnt()
	{
		return _queue.size();
	}

	bool push_msg(T &msg)
	{
		bool res = false;
		_queue_lock->lock();
		if(_queue.size() == 0)
		{
			_queue.push_back(msg);
			_queue_cond->signal();
			res = true;
		}
		else if(_queue.is_full() == true)
		{
			res = false;
		}
		else
		{
			_queue.push_back(msg);
			res = true;
		}

		_queue_lock->unlock();
		return res;
	}
	bool pop_msg(T &msg, int timeout = -1/*millisec*/)
	{
		bool res = false;
		_queue_lock->lock();
		if (_queue.is_empty() == true)
		{
			_queue_cond->wait(_queue_lock, timeout);
			if (_queue.size() > 0)
			{
				msg = _queue.pop_front();
				res = true;
			}
		}
		else
		{
			msg = _queue.pop_front();
			res = true;
		}

		_queue_lock->unlock();

		return res;
	}
	void reset()
	{
		_queue_lock->lock();
		_queue_cond->signal();
		_queue_lock->unlock();
	}
protected:
	char _queue_name[MAX_MSG_QUEUE_NAME_SIZE];
	TCircularQueue<T> _queue;
	TMutex *_queue_lock;
	TCondition *_queue_cond;

};



