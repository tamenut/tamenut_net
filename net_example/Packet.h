#pragma once
#include "SerializedPkt.h"
#pragma pack(push, 1)
struct ClientSendPkt :public TAMENUT::SerializedPkt {
	int val;
	ClientSendPkt() : TAMENUT::SerializedPkt(1){
	}
};

struct ServerSendPkt :public TAMENUT::SerializedPkt {
	int val;
	ServerSendPkt() : TAMENUT::SerializedPkt(2){
	}
};
#pragma pack(pop)