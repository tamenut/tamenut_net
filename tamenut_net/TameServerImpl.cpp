#include "stdafx.h"
#include "TameServerImpl.h"
#include "LogModule.h"

namespace TAMENUT {
TameServerImpl::TameServerImpl()
{
}


TameServerImpl::~TameServerImpl()
{
}

void TameServerImpl::on_connect(ClientId netId)
{
	INFO_LOG("- connect\n");
}
void TameServerImpl::on_disconnect(ClientId netId, const TCHAR* pReason)
{
	INFO_LOG("- disconnect\n");
}
void TameServerImpl::on_message(ClientId netId, const TNetData& Packet)
{
	INFO_LOG("- message\n");
}
}
