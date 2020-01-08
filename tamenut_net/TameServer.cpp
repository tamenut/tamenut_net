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

void TameServer::send(unsigned int cid, SerializedPkt * pkt)
{
	_impl->post(cid, (char*)&pkt, sizeof(pkt));
}

/*
void TameServer::on_connect(unsigned int cid)
{
	INFO_LOG("- connect client (id:%u) \n", cid);
}
void TameServer::on_disconnect(unsigned int cid, const TCHAR* pReason)
{
	INFO_LOG("- disconnect\n");
}
void TameServer::on_message(unsigned int cid, const SerializedPkt& Packet)
{
	INFO_LOG("- message\n");
}
*/
}