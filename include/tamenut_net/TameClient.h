#pragma once
#include "define_dll.h"
#include "SerializedPkt.h"

namespace TAMENUT {
class TameClientImpl;
class TameClient
{
private:
	TameClientImpl *_impl;
public:
	DLL_FUNC TameClient(const char *dst_ip_str, unsigned short bind_port);
	DLL_FUNC virtual ~TameClient();

	DLL_FUNC void start();

	DLL_FUNC void send(SerializedPkt * pkt);
	DLL_FUNC virtual void on_connect() = 0;
	DLL_FUNC virtual void on_disconnect(const TCHAR * pReason) = 0;
	DLL_FUNC virtual void on_message(const SerializedPkt * pkt) = 0;
};
}
