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

	DLL_FUNC void send(unsigned int cid, SerializedPkt * pkt);
	DLL_FUNC virtual void on_connect(unsigned int cid) = 0;
	DLL_FUNC virtual void on_disconnect(unsigned int cid, const TCHAR * pReason) = 0;
	DLL_FUNC virtual void on_message(unsigned int cid, SerializedPkt *Packet) = 0;
};
}
