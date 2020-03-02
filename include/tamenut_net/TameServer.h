#pragma once
#include "define_dll.h"
#include "SerializedPkt.h"

namespace TAMENUT {
class TameServerImpl;
class TameServer
{
private:
	TameServerImpl *_impl;
public:
	DLL_FUNC TameServer(unsigned short bind_port);
	DLL_FUNC virtual ~TameServer();

	DLL_FUNC void start();

	DLL_FUNC void send(unsigned int client_id, SerializedPkt * pkt, size_t pkt_len);
	DLL_FUNC virtual void on_connect(unsigned int client_id) = 0;
	DLL_FUNC virtual void on_disconnect(unsigned int client_id, const TCHAR * pReason) = 0;
	DLL_FUNC virtual void on_message(unsigned int client_id, SerializedPkt *Packet) = 0;
};
}
