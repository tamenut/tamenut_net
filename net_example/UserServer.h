#pragma once
#include "Packet.h"
#include "TameServer.h"


class UserServer : public TAMENUT::TameServer
{
private:

public:
	UserServer(unsigned short bind_port);
	virtual ~UserServer();

	virtual void on_connect(unsigned int cid);
	virtual void on_disconnect(unsigned int cid, const TCHAR * pReason);
	virtual void on_message(unsigned int cid, TAMENUT::SerializedPkt * pkt);

};
