#include "stdafx.h"
#include "TameServerImpl.h"
#include "ServerSocket.h"
#include "LogModule.h"

namespace TAMENUT {
TameServerImpl::TameServerImpl(unsigned short bind_port)
{
	_sock = new ServerSocket(bind_port);
	_sock->set_listener(this);
}

TameServerImpl::~TameServerImpl()
{
}

void TameServerImpl::on_connect(unsigned int cid)
{
	INFO_LOG("- connect\n");
}
void TameServerImpl::on_disconnect(unsigned int cid, const TCHAR* pReason)
{
	INFO_LOG("- disconnect\n");
}
void TameServerImpl::on_message(unsigned int cid, const TNetData& Packet)
{
	INFO_LOG("- message\n");
}
}
