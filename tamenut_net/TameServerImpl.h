#pragma once
//#include "ServerSocket.h"
#include "common_def.h"
#include "TNetData.h"

namespace TAMENUT {
class ServerSocket;
class TameServerImpl
{
private:
	ServerSocket *_sock;

public:
	TameServerImpl(unsigned short bind_port);
	~TameServerImpl();
	void on_connect(unsigned int cid);
	void on_disconnect(unsigned int cid, const TCHAR * pReason);
	void on_message(unsigned int cid, const TNetData & Packet);
};
}
