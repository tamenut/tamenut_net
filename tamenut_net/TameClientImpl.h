#pragma once
#include "SerializedPkt.h"

namespace TAMENUT {
class ClientSocket;
class TameClientImpl
{
private:
	ClientSocket *_sock;
public:
	TameClientImpl(const char *dst_ip_str, unsigned short bind_port);
	~TameClientImpl();

	void send(SerializedPkt & pkt);

	void on_connect();
	void on_disconnect(const TCHAR * pReason);
	//void on_message(const SerializedPkt & Packet);
	void on_message(const SerializedPkt & Packet);
	
};

}