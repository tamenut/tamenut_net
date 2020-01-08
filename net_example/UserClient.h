#pragma once
#include "Packet.h"
#include "TameClient.h"

class UserClient : public TAMENUT::TameClient
{
private:
	
public:
	UserClient(const char *dst_ip_str, unsigned short bind_port);
	virtual ~UserClient();

	virtual void on_connect();
	virtual void on_disconnect(const TCHAR * pReason);
	virtual void on_message(TAMENUT::SerializedPkt * pkt);
	
};

