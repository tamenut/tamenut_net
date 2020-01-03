#pragma once

#define INIT_QUEUE_SIZE 1024
#include "CommonDllDefine.h"
#include <stdio.h>
#include <string.h>


class TStringCircularQueue
{
private:
	//데이터의 시작을 가리킴
	unsigned int _front;

	//데이터의 끝을 가리킴
	unsigned int _rear;

	//전체 사이즈
	unsigned int _max_size;

	//현재 사이즈
	unsigned int _cur_size;

	//내부 버퍼
	char* _queue;

	//full시에 자동 증가여부 결정
	bool _unlimited;

	unsigned int _string_cnt;

public:

	UTIL_DLLFunction TStringCircularQueue(int size = INIT_QUEUE_SIZE, bool unlimited = false);
	//복사 생성자
	UTIL_DLLFunction TStringCircularQueue(const TStringCircularQueue& v);

	UTIL_DLLFunction ~TStringCircularQueue();

	UTIL_DLLFunction void reserve(int size);
	
	UTIL_DLLFunction void set_unlimited(bool flag);
	//모든 값을 삭제하는 함수
	UTIL_DLLFunction void clear();

	UTIL_DLLFunction bool push_string(char *payload, unsigned int payload_len);

	UTIL_DLLFunction void resize(int new_size);

	UTIL_DLLFunction bool pop_string(char *payload, unsigned int &payload_len);

	UTIL_DLLFunction void copy(const TStringCircularQueue& v);

	//비어 있는 지 체크
	UTIL_DLLFunction bool is_empty();

	//사이즈를 구하는 함수
	UTIL_DLLFunction size_t get_string_cnt() const;

	UTIL_DLLFunction size_t get_string_size() const;

	UTIL_DLLFunction size_t capacity() const;

private:
	void init();

	void free_queue();

	void _push_string(char * payload, unsigned int payload_len);

	void _pop_string(char * payload, unsigned int payload_len);
};
