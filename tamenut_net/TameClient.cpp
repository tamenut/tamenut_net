#include "stdafx.h"
#include "TameClient.h"


TameClient::TameClient(const char *dst_ip_str, unsigned short bind_port)
	:TameClientImpl(dst_ip_str, bind_port)
{
}


TameClient::~TameClient()
{
}
