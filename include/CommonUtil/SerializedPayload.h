#pragma once
#include "TUtil.h"
#include "CommonDllDefine.h"
#define MAX_SERIALIZED_PAYLOAD_SIZE 64*1024



class SerializedPayload
{
public:
	UTIL_DLLFunction SerializedPayload(int max_payload_len = MAX_SERIALIZED_PAYLOAD_SIZE);
	UTIL_DLLFunction ~SerializedPayload(void);

	UTIL_DLLFunction void set_payload(char *payload_ptr, unsigned int payload_len);
	UTIL_DLLFunction char *get_payload_ptr();
	UTIL_DLLFunction void set_payload_len(unsigned int len);
	UTIL_DLLFunction unsigned int get_payload_len();
	
protected:
	char _payload[MAX_SERIALIZED_PAYLOAD_SIZE];
	unsigned int _payload_len;
};

