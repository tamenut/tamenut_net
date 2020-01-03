#pragma once

#define STRING_QUEUE_SIZE 200//10*1024*1024
#define HEARDER_SIZE sizeof(unsigned int)

#include <stdio.h>
#include <string.h>
#include "TCondition.h"
#include "TMutex.h"
#include "CommonDllDefine.h"

/****************/
/*
char ���۸� �����ϱ� ���� ť
*/
class StringQueue
{
private:
	//�������� ������ ����Ŵ
	int _front;

	//�������� ���� ����Ŵ
	int _rear;

	//��ü ������
	unsigned int _max_size;

	//���� ������
	unsigned int _cur_size;

	//���� ����
	 char * _queue;

	 TCondition _queue_cond;
	 TMutex _queue_mutex;

	 unsigned int _msg_cnt;
	 unsigned int _total_msg_lengh;

public:
	UTIL_DLLFunction StringQueue(unsigned int size = STRING_QUEUE_SIZE);

	UTIL_DLLFunction void resize(unsigned int size);

	UTIL_DLLFunction void clear();

	UTIL_DLLFunction bool can_push_string(unsigned int size);

	UTIL_DLLFunction bool push_string(char * payload, unsigned int size);

	UTIL_DLLFunction unsigned int pop_string(char * payload);

private:
	bool push(char * payload, unsigned int size);
	bool pop(char * payload, unsigned int size);

};

