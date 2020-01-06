#include "stdafx.h"
#include "TameClientImpl.h"
#include "ClientSocket.h"
#include "LogModule.h"

namespace TAMENUT {
TameClientImpl::TameClientImpl(const char *dst_ip_str, unsigned short bind_port)
{
	_sock = new ClientSocket(dst_ip_str, bind_port);
}


TameClientImpl::~TameClientImpl()
{
}
}