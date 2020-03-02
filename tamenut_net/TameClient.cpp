#include "stdafx.h"
#include "TameClient.h"
#include "TameClientImpl.h"
#include "ProcPktThread.h"

namespace TAMENUT {
TameClient::TameClient(const char *dst_ip_str, unsigned short bind_port)
{
	_impl = new TameClientImpl(dst_ip_str, bind_port, this);
}

TameClient::~TameClient()
{
}

void TameClient::start()
{
	_impl->start_client();
}

void TameClient::send(SerializedPkt * pkt, size_t pkt_len)
{
	_impl->post((char*)pkt, pkt_len);
}


}