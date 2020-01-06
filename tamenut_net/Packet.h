#pragma once
#include "SerializedPkt.h"

struct RequestPkt :public TAMENUT::SerializedPkt {
	RequestPkt() : TAMENUT::SerializedPkt(1){

	}
};

struct ReponsePkt :public TAMENUT::SerializedPkt {
	ReponsePkt() : TAMENUT::SerializedPkt(2){

	}
};