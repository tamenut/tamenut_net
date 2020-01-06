#pragma once

namespace TAMENUT {
class ClientSocket;
class TameClientImpl
{
private:
	ClientSocket *_sock;
public:
	TameClientImpl(const char *dst_ip_str, unsigned short bind_port);
	~TameClientImpl();

	/*void on_connect(ClientId netId);
	void on_disconnect(ClientId netId, const TCHAR * pReason);
	void on_message(ClientId netId, const TNetData & Packet);*/
};

}