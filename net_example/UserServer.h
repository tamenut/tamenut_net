#pragma once
#include "Packet.h"
#include "TameServer.h"


class UserServer : public TAMENUT::TameServer
{
private:

public:
	UserServer(unsigned short bind_port);
	virtual ~UserServer();

	virtual void on_connect(unsigned int client_id);
	virtual void on_disconnect(unsigned int client_id, const TCHAR * pReason);
	virtual void on_message(unsigned int client_id, TAMENUT::SerializedPkt * pkt);

};
