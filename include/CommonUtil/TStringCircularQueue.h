#pragma once

#define INIT_QUEUE_SIZE 1024
#include "CommonDllDefine.h"
#include <stdio.h>
#include <string.h>


class TStringCircularQueue
{
private:
	//�������� ������ ����Ŵ
	unsigned int _front;

	//�������� ���� ����Ŵ
	unsigned int _rear;

	//��ü ������
	unsigned int _max_size;

	//���� ������
	unsigned int _cur_size;

	//���� ����
	char* _queue;

	//full�ÿ� �ڵ� �������� ����
	bool _unlimited;

	unsigned int _string_cnt;

public:

	UTIL_DLLFunction TStringCircularQueue(int size = INIT_QUEUE_SIZE, bool unlimited = false);
	//���� ������
	UTIL_DLLFunction TStringCircularQueue(const TStringCircularQueue& v);

	UTIL_DLLFunction ~TStringCircularQueue();

	UTIL_DLLFunction void reserve(int size);
	
	UTIL_DLLFunction void set_unlimited(bool flag);
	//��� ���� �����ϴ� �Լ�
	UTIL_DLLFunction void clear();

	UTIL_DLLFunction bool push_string(char *payload, unsigned int payload_len);

	UTIL_DLLFunction void resize(int new_size);

	UTIL_DLLFunction bool pop_string(char *payload, unsigned int &payload_len);

	UTIL_DLLFunction void copy(const TStringCircularQueue& v);

	//��� �ִ� �� üũ
	UTIL_DLLFunction bool is_empty();

	//����� ���ϴ� �Լ�
	UTIL_DLLFunction size_t get_string_cnt() const;

	UTIL_DLLFunction size_t get_string_size() const;

	UTIL_DLLFunction size_t capacity() const;

private:
	void init();

	void free_queue();

	void _push_string(char * payload, unsigned int payload_len);

	void _pop_string(char * payload, unsigned int payload_len);
};
