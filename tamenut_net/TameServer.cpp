#include "stdafx.h"
#include "TameServer.h"
#include "TameServerImpl.h"

namespace TAMENUT {
TameServer::TameServer(unsigned short bind_port)
{
	_impl = new TameServerImpl(bind_port, this);
}


TameServer::~TameServer()
{
}

void TameServer::start()
{
	_impl->start_server();
}

void TameServer::send(unsigned int client_id, SerializedPkt * pkt, size_t pkt_len)
{
	_impl->post(client_id, (char*)&pkt, pkt_len);
}

}